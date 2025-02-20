

#include "cellular.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>


typedef struct s_view
{
	wchar_t	*table;
	wchar_t	*new_table;
	int		width;
	int		height;
	int		len;
} t_view;

wchar_t	*table_dup(t_view *v)
{
	int		i;
	wchar_t	*table;

	i = 0;
	table = malloc((sizeof(wchar_t)) * v->len + 1);
	if (!table)
		return (NULL);
	i = -1;
	while (v->table[++i])
		table[i] = v->table[i];
	table[i] = '\0';
	return (table);
}

wchar_t *table_create(int line, int col)
{
    int i;
    wchar_t *table;

    table = (wchar_t *)malloc(sizeof(wchar_t) * (line * col + 1));
    for (i = 0; i < line * col; i++)
       table[i] = L'□';  // Corrected string to wide character
	table[i] = L'\0';  // Null-terminate the row

    return table;
}

void print_table(t_view *v)
{
    int i = 0;

	wprintf(L"%lc ", v->table[i]); 
	i++;
    while (v->table[i] != L'\0')
    {
        if (i % v->width == 0)
			wprintf(L"\n");
		wprintf(L"%lc ", v->table[i]);
		i++;
    }
	wprintf(L"\n");
	wprintf(L"\n");
}

wchar_t	get_cell(t_view *v, int x, int y)
{
	int index = y * v->width + x;

	if (index < 0 || v->len <= index)
		return L'□';
	return v->table[index];
}

void	set_cell(t_view *v, int x, int y, wchar_t c)
{
	int index = y * v->width + x;
	v->new_table[index] = c;
}


int	count_side(t_view *v, int x, int y)
{
	int count = 0;

	count += get_cell(v, x - 1, y + 1) == L'■';
	count += get_cell(v, x - 1, y) == L'■';
	count += get_cell(v, x - 1, y - 1) == L'■';
	count += get_cell(v, x, y - 1) == L'■';
	count += get_cell(v, x + 1, y - 1) == L'■';
	count += get_cell(v, x + 1, y) == L'■';
	count += get_cell(v, x + 1, y + 1) == L'■';
	count += get_cell(v, x, y + 1) == L'■';

	return (count);
}

void algo_one(t_view *v, int x, int y)
{
	int c;
	c = count_side(v, x, y);

	if (c == 2)
	{
		return;
	}
	if (c == 3)
	{
		set_cell(v, x, y, L'■');
		return ;
	}
	set_cell(v, x, y, L'□');
}


void	update_frame(t_view *v)
{
	int i = 0;
	int x = 0;
	int y = 0;

	v->new_table = table_dup(v);
	while (v->table[i])
	{
		algo_one(v, x, y);
		x++;
		if (x == v->width)
		{
			x = 0;
			y++;	
		}
		i++;
	}
	free(v->table);
	v->table = v->new_table;
}

t_view	gen_cells(t_view v)
{
	int	i = v.len / 2 + v.width / 2;
	int j = v.len / 2 + v.width / 4; 
	v.table[i] = L'■';
	v.table[i + v.width] = L'■';
	v.table[i + v.width + 2] = L'■';
	v.table[i + (v.width * 2)] = L'■';
	v.table[i + v.width - 3] = L'■';

	v.table[j] = L'■';
	v.table[j - v.width ] = L'■';
	v.table[j - v.width - 1] = L'■';
	v.table[j + v.width + 1] = L'■';
	return v;
}

t_view	gen_cells_2(t_view v)
{
	int i = v.len / 4;

	while (i < v.len / 2)
	{
		v.table[i] = L'■';
		i+= 3;
	}
	return (v);
}

t_view	gen_cells_3(t_view v)
{
	int i = v.len / 5;

	while (i < v.len / 2 + v.len / 4)
	{
		v.table[i] = L'■';
		if ( i % 2 == 0)
			i+= 3;
		else
			i += 5;
	}
	return (v);
}

int main(int argc, char **argv)
{
	t_view v;
	int	i;
	int frames = 400;

	setlocale(LC_ALL, "");

	v.width  = atoi(argv[1]);
	v.height = atoi(argv[2]);
	v.len = v.width * v.height;
    v.table = table_create(v.height, v.width);
	v = gen_cells_3(v);
	// v.table[i] = L'■';
	// v.table[1325 + v.width] = L'■';
	// v.table[1325 + v.width + 1] = L'■';
	// v.table[1325 + (v.width * 2) + 1] = L'■';
	// v.table[1325 + v.width + 2] = L'■';
	

	print_table(&v);
	usleep(1000 * 1000);
	while(frames--)
	{
		update_frame(&v);
		print_table(&v);
		usleep(200 * 1000);
	}
    free(v.table);
    return 0;
}

