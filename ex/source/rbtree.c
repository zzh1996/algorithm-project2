#include<stdio.h>
#include<stdlib.h>
#include<time.h>

enum node_colors{
	BLACK,
	RED
};

struct node{
	enum node_colors color;
	int key;
	struct node *left;
	struct node *right;
	struct node *p;
	int size;
};

struct node node_NIL;
struct node *NIL=&node_NIL;
struct node *root;


void left_rotate(struct node *x){
	struct node *y=x->right;
	x->right=y->left;
	if(y->left!=NIL)
		y->left->p=x;
	y->p=x->p;
	if(x->p==NIL)
		root=y;
	else if(x==x->p->left)
		x->p->left=y;
	else
		x->p->right=y;
	y->left=x;
	x->p=y;
	y->size=x->size; //y的size置为x的size
	x->size=x->left->size+x->right->size+1; //重新计算x的size
}

void right_rotate(struct node *x){
	struct node *y=x->left;
	x->left=y->right;
	if(y->right!=NIL)
		y->right->p=x;
	y->p=x->p;
	if(x->p==NIL)
		root=y;
	else if(x==x->p->right)
		x->p->right=y;
	else
		x->p->left=y;
	y->right=x;
	x->p=y;
	y->size=x->size; //y的size置为x的size
	x->size=x->left->size+x->right->size+1; //重新计算x的size
}

void rb_insert_fixup(struct node *z){
	struct node *y;
	while(z->p->color==RED){
		if(z->p==z->p->p->left){
			y=z->p->p->right;
			if(y->color==RED){
				z->p->color=BLACK;
				y->color=BLACK;
				z->p->p->color=RED;
				z=z->p->p;
			}else{
				if(z==z->p->right){
					z=z->p;
					left_rotate(z);
				}
				z->p->color=BLACK;
				z->p->p->color=RED;
				right_rotate(z->p->p);
			}
		}else if(z->p==z->p->p->right){
			y=z->p->p->left;
			if(y->color==RED){
				z->p->color=BLACK;
				y->color=BLACK;
				z->p->p->color=RED;
				z=z->p->p;
			}else{
				if(z==z->p->left){
					z=z->p;
					right_rotate(z);
				}
				z->p->color=BLACK;
				z->p->p->color=RED;
				left_rotate(z->p->p);
			}
		}
		root->color=BLACK;
	}
}

void rb_insert(struct node *z){
	struct node *y=NIL;
	struct node *x=root;
	while(x!=NIL){
		y=x;
		x->size++; //插入新结点路上的size都+1
		if(z->key<x->key)
			x=x->left;
		else
			x=x->right;
	}
	z->p=y;
	if(y==NIL)
		root=z;
	else if(z->key<y->key)
		y->left=z;
	else
		y->right=z;
	z->left=NIL;
	z->right=NIL;
	z->color=RED;
	z->size=1; //新结点的size为1
	rb_insert_fixup(z);
}

void rb_transplant(struct node *u,struct node *v){
	if(u->p==NIL)
		root=v;
	else if(u==u->p->left)
		u->p->left=v;
	else
		u->p->right=v;
	v->p=u->p;
}

struct node *tree_minimum(struct node *x){
	while(x->left!=NIL)
		x=x->left;
	return x;
}

void rb_delete_fixup_size(struct node *x){ //删除结点时维护size
	while(x!=root){
		x=x->p;
		x->size--;
	}
}

void rb_delete_fixup(struct node *x){
	struct node *w;
	while(x!=root&&x->color==BLACK){
		if(x==x->p->left){
			w=x->p->right;
			if(w->color==RED){
				w->color=BLACK;
				x->p->color=RED;
				left_rotate(x->p);
				w=x->p->right;
			}
			if(w->left->color==BLACK&&w->right->color==BLACK){
				w->color=RED;
				x=x->p;
			}else{
				if(w->right->color==BLACK){
					w->left->color=BLACK;
					w->color=RED;
					right_rotate(w);
					w=x->p->right;
				}
				w->color=x->p->color;
				x->p->color=BLACK;
				w->right->color=BLACK;
				left_rotate(x->p);
				x=root;
			}
		}else if(x==x->p->right){
			w=x->p->left;
			if(w->color==RED){
				w->color=BLACK;
				x->p->color=RED;
				right_rotate(x->p);
				w=x->p->left;
			}
			if(w->right->color==BLACK&&w->left->color==BLACK){
				w->color=RED;
				x=x->p;
			}else{
				if(w->left->color==BLACK){
					w->right->color=BLACK;
					w->color=RED;
					left_rotate(w);
					w=x->p->left;
				}
				w->color=x->p->color;
				x->p->color=BLACK;
				w->left->color=BLACK;
				right_rotate(x->p);
				x=root;
			}
		}
	}
	x->color=BLACK;
}

void rb_delete(struct node *z){
	struct node *y=z;
	struct node *x;
	enum node_colors y_original_color=y->color;
	if(z->left==NIL){
		rb_delete_fixup_size(y); //删除结点时y原位置到根的size都-1
		x=z->right;
		rb_transplant(z,z->right);
	}else if(z->right==NIL){
		rb_delete_fixup_size(y); //删除结点时y原位置到根的size都-1
		x=z->left;
		rb_transplant(z,z->left);
	}else{
		y=tree_minimum(z->right);
		rb_delete_fixup_size(y); //删除结点时y原位置到根的size都-1
		y_original_color=y->color;
		x=y->right;
		if(y->p==z){
			x->p=y;
		}else{
			rb_transplant(y,y->right);
			y->right=z->right;
			y->right->p=y;
		}
		rb_transplant(z,y);
		y->left=z->left;
		y->left->p=y;
		y->color=z->color;
		y->size=z->size; //y的新size即为z的size
	}
	if(y_original_color==BLACK)
		rb_delete_fixup(x);
}

struct node *os_select(struct node *x,int i){ //查找具有给定秩的元素
	int r=x->left->size+1;
	if(i==r)
		return x;
	else if(i<r)
		return os_select(x->left,i);
	else
		return os_select(x->right,i-r);
}

int os_rank(struct node *x){ //确定一个元素的秩
	int r=x->left->size+1;
	struct node *y=x;
	while(y!=root){
		if(y==y->p->right)
			r+=y->p->left->size+1;
		y=y->p;
	}
	return r;
}

struct node *new_node(int k){ //创建key为k的新结点
	struct node *p=(struct node *)malloc(sizeof(struct node));
	p->color=BLACK;
	p->key=k;
	p->left=NULL;
	p->right=NULL;
	p->p=NULL;
	p->size=0;
	return p;
}

void preorder(struct node *p,FILE *fp){ //先序遍历
	if(p==NIL)
		return;
	fprintf(fp,"%d\n",p->key);
	preorder(p->left,fp);
	preorder(p->right,fp);
}

void inorder(struct node *p,FILE *fp){ //中序遍历
	if(p==NIL)
		return;
	inorder(p->left,fp);
	fprintf(fp,"%d\n",p->key);
	inorder(p->right,fp);
}

void postorder(struct node *p,FILE *fp){ //后序遍历
	if(p==NIL)
		return;
	postorder(p->left,fp);
	postorder(p->right,fp);
	fprintf(fp,"%d\n",p->key);
}

void output(struct node *p){ //打印树的结构详细信息，用于画图
	if(p==NIL)
		return;
	output(p->left);
	printf("  %d L=%d R=%d P=%d color=%c size=%d\n",p->key,p->left->key,p->right->key,p->p->key,p->color==RED?'R':'B',p->size);
	output(p->right);
}

int main(){
	FILE *fp;
	int data[80]; //保存读入的数据
	struct node *nodes[10]; //保存新建的结点
	int i,j;
	int n;
	char path[100]; //文件名

	double diff,total;
	struct timespec start,stop;

	struct node *node1,*node2; //保存待删除的结点

	node_NIL.color=BLACK;
	node_NIL.key=-1;
	node_NIL.left=NULL;
	node_NIL.right=NULL;
	node_NIL.p=NULL;
	node_NIL.size=0;

	//从文件读入数据
	fp=fopen("../input/input.txt","rt");
	for(i=0;i<80;i++){
		fscanf(fp,"%d",&data[i]);
	}
	fclose(fp);

	//循环每种数据规模
	for(n=20;n<=80;n+=20){
		sprintf(path,"../output/size%d/time1.txt",n);
		fp=fopen(path,"wt");
		root=NIL;
		total=0;
		for(i=0;i<n;i+=10){ //每10个计时
			for(j=0;j<10;j++)
				nodes[j]=new_node(data[i+j]); //准备要插入的结点
			clock_gettime(CLOCK_MONOTONIC,&start); //开始计时
			for(j=0;j<10;j++)
				rb_insert(nodes[j]);
			clock_gettime(CLOCK_MONOTONIC,&stop); //结束计时
			diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
			total+=diff;
			fprintf(fp,"insert %d~%d %.9fs\n",i,i+9,diff);
		}
		fprintf(fp,"insert total %.9fs\n",total);
		fclose(fp);

		//先序遍历
		sprintf(path,"../output/size%d/preorder.txt",n);
		fp=fopen(path,"wt");
		preorder(root,fp);
		fclose(fp);

		//中序遍历
		sprintf(path,"../output/size%d/inorder.txt",n);
		fp=fopen(path,"wt");
		inorder(root,fp);
		fclose(fp);

		//后序遍历
		sprintf(path,"../output/size%d/postorder.txt",n);
		fp=fopen(path,"wt");
		postorder(root,fp);
		fclose(fp);

		//找到第n/4和n/2小的结点
		node1=os_select(root,n/4);
		node2=os_select(root,n/2);

		sprintf(path,"../output/size%d/delete_data.txt",n);
		fp=fopen(path,"wt");
		fprintf(fp,"%d\n%d\n",node1->key,node2->key);
		fclose(fp);

		sprintf(path,"../output/size%d/time2.txt",n);
		fp=fopen(path,"wt");
		if(n==20){ //n=20时输出树的结构用于画图
			printf("before delete");
			output(root);
		}
		//删除结点计时
		clock_gettime(CLOCK_MONOTONIC,&start);
		rb_delete(node1); //删除n/4
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		fprintf(fp,"delete node1 %.9fs\n",diff);
		if(n==20){
			printf("after delete node1");
			output(root);
		}
		clock_gettime(CLOCK_MONOTONIC,&start);
		rb_delete(node2); //删除n/2
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		fprintf(fp,"delete node2 %.9fs\n",diff);
		if(n==20){
			printf("after delete node2");
			output(root);
		}
		fclose(fp);

		sprintf(path,"../output/size%d/delete_inorder.txt",n);
		fp=fopen(path,"wt");
		inorder(root,fp); //删除后的中序遍历
		fclose(fp);
	}

	return 0;
}