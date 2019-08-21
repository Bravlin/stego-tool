#include "plot.h"

//#define ROOT_DIRECTORY "/home/Desktop/"

/** 
 * @param x_value : x values
 * @param y_value : y values
 * @param count : number of value pairs
 * @param x_label : x label
 * @param y_label : y label
 * @param title : graph title
 * @param output_filename : name for the output file
 */
void plot(float *x_value, int *y_value, int count, char *x_label, char *y_label, char *title, char *output_filename)
{
    int i;
    char *file_ext = ".jpeg ";
    char *title_suffix = "set title \'";
    char *xlabel_suffix = "set xlabel \'";
    char *ylabel_suffix = "set ylabel \'";
    char aux[500];
    FILE *temp_file = fopen("data.temp", "w");

    for (i = 0; i < count; i++)
    {
        fprintf(temp_file, "%lf %d \n", x_value[i], y_value[i]);
    }

    fclose(temp_file);
    FILE *commands_file = fopen("gnuplot-commands.txt", "w");
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
    fclose(commands_file);

    strcpy(aux, "gnuplot ");
    strcat(aux, "gnuplot-commands.txt > ");
    strcat(aux, output_filename);
    strcat(aux, file_ext);
    printf("%s\n", aux);
    system(aux);
}

int main()
{
    float x[10];
    int y[10];
    int N = 10;
    int i;

    for (i = 0; i < N; i++)
    {
        x[i] = (i * 2);
        y[i] = (i / 2);
    }

    plot(x, y, N, "valores x", "valores y", "MSE", "mse");
    plot(x, y, N, "valores x", "valores y", "PSNR", "psnr");
    plot(x, y, N, "valores x", "valores y", "SSIM", "ssim");
    return 0;
}