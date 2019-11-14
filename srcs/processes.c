#include "../inc/ft_ssl.h"

static t_parse_msg_fn	get_msg_fn(t_type type, unsigned int i)
{
	if (g_parser_handlers[i].type == type)
		return (g_parser_handlers[i].parse_fn);
	return (get_msg_fn(type, i + 1));
}

static t_process		*init_process(char *input, t_type type)
{
	t_process	*process;

	process = malloc(sizeof(t_process));
	process->type = type;
	process->input = input;
	process->parse_msg_fn = get_msg_fn(type, 0);
	process->next = NULL;
	return (process);
}

t_process				*push_process(t_process *head, char *input, t_type type)
{
	if (head == NULL)
		return (init_process(input, type));
	head->next = push_process(head->next, input, type);
	return (head);
}

t_process				*prepend_process(t_process *h, char *input, t_type type)
{
	t_process	*process;

	process = init_process(input, type);
	process->next = h;
	return (process);
}

void					run_processes(t_handler *handler, t_process *head)
{
	char		*hashed;
	t_content	*to_hash;
	char		*display_input;
	size_t		len;

	if (head == NULL)
		return ;
	to_hash = head->parse_msg_fn(head->input);
	if (to_hash != NULL)
	{
		display_input = head->type == H_STDIN ? to_hash->content : head->input;
		len = head->type == H_STDIN ? to_hash->size : ft_strlen(head->input);
		hashed = handler->hash_fn(to_hash);
		display(handler, head, hashed, display_input, len);
	}
	free_content(to_hash);
	run_processes(handler, head->next);
}
