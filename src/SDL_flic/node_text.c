#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"node_text.h"

static node_text *first_node_text=NULL;
static node_text *actual_node_text=NULL;


static inline int getNum(char *text)
{
	int ret=0;
	int i,j,o,l=strlen(text)-1;

	for(i=0;i<l;i++)
		if ((text[i]<'0')||(text[i]>'9'))
		{
			if (text[i]==' ')
				break;
			else
				return 0;
		}
	o=i+1;
	for(i--,j=1;i>=0;i--,j*=10)
		ret+=j*(text[i]-'0');
	for(i=o;i<=l;i++)
		text[i-o]=text[i];
	text[i-o]=0;
		
	return ret;
}

int node_text_read(char *file_name)
{
	char *tmp;
	void *buf;
	FILE *f=fopen(file_name,"r");
	node_text *actual;

	actual_node_text=NULL;
	if (f==NULL)
		return -1;

	tmp=(char *)calloc(1024,1);
	if (tmp==NULL)
	{
		fclose(f);
		return -2;
	}
	
	if (first_node_text!=NULL)
		free(first_node_text);
	first_node_text=NULL;
	buf=calloc(NODE_BUF_SIZE,1);
	if (buf==NULL)
	{
		fclose(f);
		free(tmp);
		return -3;
	}
	
	first_node_text=(node_text *)buf;
	actual=first_node_text;
	actual->text=(char *)(buf+sizeof(node_text));

	while(!feof(f))
	{
		actual->text=(char *)(((void *)actual)+sizeof(node_text));

		memset(tmp,0,1024);
		if (fgets(tmp,1024,f)==NULL)
			break;
		actual->frame=getNum(tmp);
		strcpy(actual->text,tmp);

		register int sum=5+strlen(actual->text);
		sum-=(sum%4);
		actual->next=(p_node_text)(((void *)actual->text)+sum);
		actual=(node_text *)actual->next;
	}
	actual->frame=0;
	actual->text=NULL;
	actual->next=NULL;

	fclose(f);
	free(tmp);
	actual_node_text=first_node_text;
	return 0;
}

int node_text_first(void)
{
	if (first_node_text==NULL)
		return -1;
	actual_node_text=first_node_text;
	return 0;
}

int node_text_get_frame(void)
{
	if (actual_node_text==NULL)
		return 0;
	return actual_node_text->frame;
}

char *node_text_get_text(void)
{
	if (actual_node_text==NULL)
		return NULL;
	return actual_node_text->text;
}

int node_text_next(void)
{
	if (actual_node_text==NULL)
		return -2;
	if (actual_node_text->next==NULL)
		return -1;
	actual_node_text=(node_text *)actual_node_text->next;
	return 0;
}

void node_text_free_buffer(void)
{
	if (first_node_text!=NULL)
		free(first_node_text);
	first_node_text=NULL;
	actual_node_text=NULL;
}

