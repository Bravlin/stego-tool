#include "plot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA_FILENAME "data.temp"
#define COMMANDS_FILENAME "gnuplot-commands.temp"

/**
 * @param x_value : x values
 * @param y_value : y values
 * @param count : number of value pairs
 * @param x_label : x label
 * @param y_label : y label
 * @param title : graph title
 * @param output_filename : name for the output file
 */
void plot(int *x_value, long double *y_value, int count, char *x_label, char *y_label, char *title, char *output_filename)
{
    int i;
    char *file_ext = ".jpeg ";
    char *title_suffix = "set title \'";
    char *xlabel_suffix = "set xlabel \'";
    char *ylabel_suffix = "set ylabel \'";
    char aux[500];

    FILE *temp_file = fopen(DATA_FILENAME, "w");

    for (i = 0; i < count; i++)
    {
        fprintf(temp_file, "%d %Lf \n", x_value[i], y_value[i]);
    }

    fclose(temp_file);

    FILE *commands_file = fopen(COMMANDS_FILENAME, "w");
    fprintf(commands_file, "set terminal jpeg\n");
    strcpy(aux, title_suffix);
    strcat(aux, title);
    strcat(aux, "\'\n");
    fprintf(commands_file, "%s", aux);
    strcpy(aux, xlabel_suffix);
    strcat(aux, x_label);
    strcat(aux, "\'\n");
    fprintf(commands_file, "%s", aux);
    strcpy(aux, ylabel_suffix);
    strcat(aux, y_label);
    strcat(aux, "\'\n");
    fprintf(commands_file, "%s", aux);
    strcpy(aux, "plot 'data.temp' with linespoints ls 1");
    fprintf(commands_file, "%s", aux);
    strcpy(aux, "gnuplot ");
    strcat(aux, COMMANDS_FILENAME);
    strcat(aux, " > ");
    strcat(aux, output_filename);
    strcat(aux, file_ext);

    fclose(commands_file);
    system(aux);
    remove(DATA_FILENAME);
    remove(COMMANDS_FILENAME);
}