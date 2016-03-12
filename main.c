#include <stdio.h>
#include <stdlib.h>

#define MAXLEVEL 10//规定最大层数
typedef int valuetype;

typedef struct node1
{
	valuetype value;
	struct node1 *next[0];
}Node;

typedef struct node
{
	int level;
	struct node1 *head;
}skip_list;

Node *new_node(int level, valuetype value)
{
	Node *p = (Node*)malloc(sizeof(Node) + level*sizeof(Node));
	if (p)
	{
		p->value = value;
		for (int i = 0;i < level;i++)
		{
			p->next[i] = NULL;
		}
		return  p;
	}
	else
		exit(-1);
}

skip_list creat_skip_list()
{
	skip_list *sl = (skip_list*)malloc(sizeof(skip_list));
	sl->level = MAXLEVEL;
	sl->head = new_node(MAXLEVEL, 0);
	return *sl;
}

int random_level()
{
	int l=1;
	while (rand() % 2)
	{
		l++;
	}
	l = (l > MAXLEVEL) ? MAXLEVEL : l;
	return l;
}

//插入函数；如果某个节点的后继结点是NULL或者后继节点的value值大于要插入节点，则该节点是下降节点，需要记录到update中。下降到该节点的第i-1层
int insert_node(skip_list *sl, valuetype value)
{
	Node *update[MAXLEVEL];//这个数组是用来保存下降的节点的；
	Node *location = NULL;//这个节点是用来记录当前的下降节点的；
	int level = random_level();//创建随机层数，即要加入的节点所占有的层数。
	int j = MAXLEVEL-1;//用于记录当前所处的层数。
	int i = 0;//用来给update计数
	location = sl->head;
	int countinue = 1;
	//确定要插入的位置,插在location之后；
	while (countinue)
	{
		while ((location->next[j] == NULL || location->next[j]->value > value) && j > 0)//这一段只用来减层。
		{
			j -= 1;
		}
		//下面用来找位置。
		update[i] = location;
		//location = location->next[j];
		i += 1;
		if (location->next[j] == NULL || location->next[j]->value > value)
		{
			countinue = 0;
		}
		else
		{
			location = location->next[j];
		}
	}
	//开始插入
	Node *p= new_node(level, value);
	//这是插入步骤，循环跳出的条件是p的所有next已经插满了或者是update数组已经到了第0个元素。
	while (j <= level-1 && i > 0)
	{
		location = update[i - 1];
		p->next[j] = location->next[j];
		location->next[j] = p;
		j += 1;
		i -= 1;
	}
	if (j <= level-1)//要把p的next全部插满
	{
		while (j <= level-1)
		{
			p->next[j] = location->next[j];
			location->next[j] = p;
			j += 1;
		}
	}
	printf("done!\n");
	return 1;
}

//首先从头的最高层往后找，如果找到了就降层，并记录location到update数组。直到第零层。
int delete_node(skip_list *sl, valuetype value)
{
	Node *update[MAXLEVEL];//这个数组是用来保存下降的节点的；
	Node *location = NULL;//这个节点是用来记录当前的下降节点的；
	int j = MAXLEVEL - 1;//j表示层数。
	int i = 0;//用来给update计数。
	location = sl->head;
	//降到0层
	do
	{
		if (location->next[j] == NULL)
		{
			
			j -= 1;
			/*
			if (location->next[j] != NULL)
			{
				location = location->next[j];
			}
			*/
		}
		else if (location->next[j]->value >= value)
		{
			if (location->next[j]->value == value)
			{
				update[i] = location;
				i += 1;
			}
			j -= 1;
		}
		else if (location->next[j] != NULL && location->next[j]->value < value)
		{
			location = location->next[j];
		}
		
	} while (j>0);
	//此时己经降到了最后一层，如果没降到的话说明前面有错误。
	while (location->next[j]->value < value)
	{
		location = location->next[j];
	}
	if (location->next[j]->value == value)
	{
		update[i] = location;
		location = location->next[j];
		//此时已经在第0层找到了要删除的点，location即为要删除的点。开始删除，没找到的情况后续处理
		while (i >= 0)
		{
			update[i]->next[j] = location->next[j];
			i--;
			j++;
		}
		return 0;
	}
	else
	{
		printf("erro!no such node!\n");
		return 0;
	}

}

//查找节点查到了返回1
int search_node(skip_list *sl, valuetype value)
{
	//Node *update[MAXLEVEL];//这个数组是用来保存下降的节点的；
	Node *location = NULL;//这个节点是用来记录当前的下降节点的；
	int j = MAXLEVEL - 1;//j表示层数。
	int i = 0;//用来给update计数。
	location = sl->head;
	//降到0层
	do
	{
		if (location->next[j] == NULL || location->next[j]->value >= value)
		{
			
			j -= 1;
			/*
			if (location->next[j] != NULL)
			{
			location = location->next[j];
			}
			*/
		}
		else if (location->next[j] != NULL && location->next[j]->value < value)
		{
			location = location->next[j];
		}

	} while (j>0);
	//此时己经降到了最后一层，如果没降到的话说明前面有错误。
	while (location->next[j]->value < value)
	{
		location = location->next[j];
	}
	if (location->next[j]->value == value)
	{
		
		return 1;
	}
	else
		
		return 0;
		
	
}


int main()
{
	skip_list sl = creat_skip_list();
	int choose, is_cont, value;
	do {
		printf("choose options:\n1.insert\n2.delete\n3.search\n");
		scanf("%d", &choose);
		switch (choose)
		{
		case 1:
			printf("input a number to insert:");
			scanf("%d",&value);
			insert_node(&sl, value);
			break;
		case 2:
			printf("input a number to delete:");
			scanf("%d",&value);
			delete_node(&sl, value);
			break;
		case 3:
			printf("input a number to search:");
			scanf("%d",&value);
			if (search_node(&sl, value))
			{
				printf("find the node!\n");
			}
			else
				printf("no such node!\n");
			break;
		default:
			printf("input erro!\n");
			break;
		}
		printf("continue?\n1/0\n");
		scanf("%d", &is_cont);
	} while (is_cont);
	return 0;
}