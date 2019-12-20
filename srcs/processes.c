#include "../inc/ft_ssl.h"

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

void					free_processes(t_process *head)
{
	if (head == NULL)
		return ;
	free_processes(head->next);
	free(head);
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
	t_content	to_display;

	if (head == NULL)
	{
		return ;
	}
	to_hash = head->parse_msg_fn(head->input);
	if (to_hash != NULL)
	{
		to_display.content = head->type == H_STDIN ?
			to_hash->content : head->input;
		to_display.size = head->type == H_STDIN ?
			to_hash->size : ft_strlen(head->input);
		hashed = handler->hash_fn(to_hash);
		display(handler, head, hashed, to_display);
		free(hashed);
	}
	free_content(to_hash);
	run_processes(handler, head->next);
}
