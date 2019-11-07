#include "../inc/ft_ssl.h"

static t_parse_msg_fn	get_msg_fn_aux(t_type type, unsigned int i)
{
	if (g_parser_handlers[i].type == type)
	{
		printf("(debug) parse function found\n");
		return (g_parser_handlers[i].parse_fn);
	}
	return (get_msg_fn_aux(type, i + 1));
}

static t_parse_msg_fn	get_msg_fn(t_type type)
{
	return (get_msg_fn_aux(type, 0));
}

static t_process		*init_process(char *input, t_type type)
{
	t_process	*process;

	printf("(debug) input: %s\n", input);
	process = malloc(sizeof(t_process));
	process->input = input;
	process->parse_msg_fn = get_msg_fn(type);
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

void					run_processes(t_handler *handler, t_process *head)
{
	char	*hashed;
	char	*to_hash;

	if (head == NULL)
		return ;
	to_hash = head->parse_msg_fn(head->input);
	printf("(debug) to_hash: %s\n", to_hash);
	if (to_hash != NULL)
	{
		hashed = handler->hash_fn(to_hash);
		display(handler, hashed);
	}
	run_processes(handler, head->next);
}
