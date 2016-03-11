#include <stdio.h>
#include <stdlib.h>

#define MAXLEVEL 10//�涨������
typedef int valuetype;

typedef struct node
{
	valuetype value;
	Node *next[0];
}Node;

typedef struct node
{
	int level;
	Node *head;
}skip_list;

Node *new_node(int level, valuetype value)
{
	Node *p = (Node*)malloc(sizeof(Node) + level*sizeof(Node));
	if (!p)
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
	sl->level = 0;
	sl->head = new_node(MAXLEVEL, 0);
	return *sl;
}

int random_level()
{
	int l;
	while (rand() % 2)
	{
		l++;
	}
	l = (l > MAXLEVEL) ? MAXLEVEL : l;
	return l;
}

//���뺯�������ĳ���ڵ�ĺ�̽����NULL���ߺ�̽ڵ��valueֵ����Ҫ����ڵ㣬��ýڵ����½��ڵ㣬��Ҫ��¼��update�С��½����ýڵ�ĵ�i-1��
int insert_node(skip_list *sl, valuetype value)
{
	Node *update[MAXLEVEL];//������������������½��Ľڵ�ģ�
	Node *location = NULL;//����ڵ���������¼��ǰ���½��ڵ�ģ�
	int level = random_level();//���������������Ҫ����Ľڵ���ռ�еĲ�����
	int j = MAXLEVEL-1;//���ڼ�¼��ǰ�����Ĳ�����
	int i = 0;//������update����
	location = sl->head;
	int countinue = 1;
	//ȷ��Ҫ�����λ��,����location֮��
	while (countinue)
	{
		while ((location->next[j] == NULL || location->next[j]->value > value) && j > 0)//��һ��ֻ�������㡣
		{
			j -= 1;
		}
		//����������λ�á�
		update[i] = location;
		location = location->next[j];
		i += 1;
		if (j == 0 && (location->next[j]->value>value || location->next[j]==NULL))
		{
			countinue = 0;
		}
	}
	//��ʼ����
	Node *p= new_node(level, value);
	//���ǲ��벽�裬ѭ��������������p������next�Ѿ������˻�����update�����Ѿ����˵�0��Ԫ�ء�
	while (j <= level-1 && i > 0)
	{
		location = update[i - 1];
		p->next[j] = location->next[j];
		location->next[j] = p;
		j += 1;
		i -= 1;
	}
	if (j <= level-1)//Ҫ��p��nextȫ������
	{
		while (j <= level-1)
		{
			p->next[j] = location->next[j];
			location->next[j] = p;
			j += 1;
		}
	}
	return 1;
}

//���ȴ�ͷ����߲������ң�����ҵ��˾ͽ��㣬����¼location��update���顣ֱ������㡣
int delete_node(skip_list *sl, valuetype value)
{
	Node *update[MAXLEVEL];//������������������½��Ľڵ�ģ�
	Node *location = NULL;//����ڵ���������¼��ǰ���½��ڵ�ģ�
	int j = MAXLEVEL - 1;//j��ʾ������
	int i = 0;//������update������
	location = sl->head;
	//����0��
	do
	{
		if (location->next[j]->value >= value|| location->next[j] == NULL)
		{
			if (location->next[j]->value == value)
			{
				update[i] = location;
				i += 1;
			}
			j -= 1;
			/*
			if (location->next[j] != NULL)
			{
				location = location->next[j];
			}
			*/
		}
		else if (location->next[j]->value < value)
		{
			location = location->next[j];
		}
		
	} while (j>0);
	//��ʱ�������������һ�㣬���û�����Ļ�˵��ǰ���д���
	while (location->next[j]->value < value)
	{
		location = location->next[j];
	}
	if (location->next[j]->value == value)
	{
		update[i] = location;
		location = location->next[j];
		//��ʱ�Ѿ��ڵ�0���ҵ���Ҫɾ���ĵ㣬��ʼɾ����û�ҵ��������������
		while (i >= 0)
		{
			update[i]->next[j] = location;
		}
	}
	




}