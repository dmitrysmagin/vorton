
typedef struct node_text *p_node_text;

typedef struct{
	int frame;
	char *text;
	p_node_text next;
}node_text;

#define NODE_BUF_SIZE (1024*16)

int node_text_read(char *file_name);
int node_text_first(void);
int node_text_get_frame(void);
char *node_text_get_text(void);
int node_text_next(void);
void node_text_free_buffer(void);
