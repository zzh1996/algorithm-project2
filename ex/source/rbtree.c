#include<stdio.h>
#include<stdlib.h>

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
	y->size=x->size;//
	x->size=x->left->size+x->right->size+1;//
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
	y->size=x->size;//
	x->size=x->left->size+x->right->size+1;//
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
		x->size++; //
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
	z->size=1;//
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

void rb_delete_fixup_size(struct node *x){
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
		rb_delete_fixup_size(y);//
		x=z->right;
		rb_transplant(z,z->right);
	}else if(z->right==NIL){
		rb_delete_fixup_size(y);//
		x=z->left;
		rb_transplant(z,z->left);
	}else{
		y=tree_minimum(z->right);
		rb_delete_fixup_size(y);//
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
		y->size=z->size;//
	}
	if(y_original_color==BLACK)
		rb_delete_fixup(x);
}

struct node *os_select(struct node *x,int i){
	int r=x->left->size+1;
	if(i==r)
		return x;
	else if(i<r)
		return os_select(x->left,i);
	else
		return os_select(x->right,i-r);
}

int os_rank(struct node *x){
	int r=x->left->size+1;
	struct node *y=x;
	while(y!=root){
		if(y==y->p->right)
			r+=y->p->left->size+1;
		y=y->p;
	}
	return r;
}

struct node *new_node(int k){
	struct node *p=(struct node *)malloc(sizeof(struct node));
	p->color=BLACK;
	p->key=k;
	p->left=NULL;
	p->right=NULL;
	p->p=NULL;
	p->size=0;
	return p;
}

void output(struct node *p){
	if(p==NIL)
		return;
	output(p->left);
	printf("%d L=%d R=%d P=%d color=%c size=%d\n",p->key,p->left->key,p->right->key,p->p->key,p->color==RED?'R':'B',p->size);
	output(p->right);
}

int main(){
	int test[]={9,5,7,4,1,3,2,8},i;
	node_NIL.color=BLACK;
	node_NIL.key=-1;
	node_NIL.left=NULL;
	node_NIL.right=NULL;
	node_NIL.p=NULL;
	node_NIL.size=0;
	root=NIL;

	for(i=0;i<sizeof(test)/sizeof(*test);i++)
		rb_insert(new_node(test[i]));
	output(root);

	rb_delete(os_select(root,6));

	output(root);

	printf("os_select %d\n",os_select(root,6)->key);
	printf("os_rank %d\n",os_rank(os_select(root,6)));

	return 0;
}