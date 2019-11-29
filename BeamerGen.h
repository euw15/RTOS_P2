#include <stdio.h>
#include <math.h>

#define MAX_TIME_COLS 12
#define MAX_TABLES_PER_SLIDE 2

char* gCellColors[MAX_TASKS+1] = 
{
    "\\cellcolor{teal}",
    "\\cellcolor{magenta}",
    "\\cellcolor{cyan}",
    "\\cellcolor{lime}",
    "\\cellcolor{orange}",
    "\\cellcolor{yellow}",
    "\\cellcolor{gray!30}"
};

void StartPresentation(FILE* fp)
{
    fputs("\\begin{document}\n"
    "\\maketitle\n\n"

    "\\begin{frame}{Contenidos}\n"
    "\\tableofcontents\n"
    "\\end{frame}\n", fp);
}

void ClosePresentation(FILE* fp)
{
    fputs("\\end{document}\n", fp);
}

void ParametersSection(FILE* fp)
{
    int i = 0;
    char* string1 = 
        "\\section{Parámetros}\n"
        "\\begin{frame}{Parámetros de ejecución }\n"
        "    \\begin{block}{}\n"
        "        \\begin{columns}[onlytextwidth,T]\n"
        "            \\column{\\dimexpr\\linewidth-30mm-5mm}\n"
        "                \\begin{table}\n"
        "                    \\begin{tabular}{c | a | b | a | b}\n"
        "                        \\hline\n"
        "                        \\rowcolor{LightCyan}\n"
        "                        \\mc{1}{Tareas}  & \\mc{1}{C\\textsubscript{i}} & \\mc{1}{T\\textsubscript{i}} \\\\ \n"
        "                        \\hline\n"
        ;
    char* string2 =
        "                    \\end{tabular}\n"
        "                \\end{table}\n\n"
        "            \\column{30mm}\n"
        "                Algoritmos seleccionados:\n"
        "                \\begin{enumerate}\n"
        ;
    char* string3 =
        "                \\end{enumerate}\n"
        "        \\end{columns}\n"
        "    \\end{block}\n"
        "\\end{frame}\n\n"
        ;
    fputs(string1, fp);
    for( i = 0; i < Total_tasks; i++)
    {
        fprintf(fp, "                        T%d & %d & %d \\\\ \n", i+1, all_Tasks[i].C, all_Tasks[i].P);
        fputs("                        \\hline\n", fp);
    }
    fputs(string2, fp);
    if(flagRM)
        fputs("                   \\item RM\n", fp);
    if(flagEDF)
        fputs("                   \\item EDF\n", fp);
    if(flagLLF)
        fputs("                   \\item LLF\n", fp);
    fputs(string3, fp);
}

void SchedAlgorithmsSection(FILE* fp)
{
    fputs("\\section{Algoritmos de calendarización}\n", fp);

    char* string1 =
    "\\begin{frame}{Rate Monotonic}\n"
    "    Algoritmo de calendarización de prioridad estática. La prioridad de cada tarea está dada según su período. A menor período, mayor prioridad.\\\\~\\\\\n"
    "    Prueba de calendarizabilidad:\n"
    "    \\begin{equation} \\label{eq:ej1}\n"
    "        U\n"
    "        =\n"
    "        \\sum_{i=1}^{n} \\frac{C_i}{T_i}\n"
    "        \\leq\n"
    "        n(2^{\\frac{1}{n}} - 1)\n"
    "    \\end{equation}\n"

    "    \\begin{equation} \\label{eq:ej2}\n"
    "        \\prod_{i=1}^{n} {(\\frac{C_i}{T_i} + 1)}\n"
    "        \\leq\n"
    "        2\n"
    "    \\end{equation}\n"
    "\\end{frame}\n\n";

    char* string2 =
    "\\begin{frame}{Earliest Deadline First}\n"
    "    Algoritmo de calendarización de prioridad dinámica. La prioridad de cada tarea está dada según la proximidad de su deadline. \n"
    "    En un momento dado, la tarea de mayor prioridad será aquella cuyo deadline esté más próximo a alcanzarse.\\\\~\\\\\n"
    "    Prueba de calendarizabilidad:\n"
    "    \\begin{equation} \\label{eq:ej3}\n"
    "        U\n"
    "        =\n"
    "        \\sum_{i=1}^{n} \\frac{C_i}{T_i}\n"
    "        <\n"
    "        1\n"
    "    \\end{equation}\n"
    "\\end{frame}\n\n";

    char* string3 =
    "\\begin{frame}{Least Laxity First}\n"
    "    Algoritmo de calendarización de prioridad dinámica. La prioridad de cada tarea está dada según la holgura en su tiempo restante de ejecución. \n"
    "    En cada momento dado, la tarea de mayor prioridad será aquella con el menor valor dado por la siguiente fórmula (holgura/laxity):\\\\~\\\\\n"
    "    \\begin{equation} \\label{eq:ej4}\n"
    "        L\\textsubscript{i}\n"
    "        =\n"
    "        D\\textsubscript{i} - (t\\textsubscript{t} + C\\textsubscript{i}\\textsuperscript{R})\n"
    "    \\end{equation}\n"
    "    \\textsl{donde D\\textsubscript{i} es el deadline de la tarea, t\\textsubscript{t} el tiempo actual, y C\\textsubscript{i}\\textsuperscript{R}\n"
    "    el tiempo de computación restante.}\n"
    "\\end{frame}\n\n"
    ;

    if(flagRM)
        fputs(string1, fp);
    if(flagEDF)
        fputs(string2, fp);
    if(flagLLF)
        fputs(string3, fp);
}

void SchedAlgorithmsTestSection(FILE* fp)
{
    double U = 0;
    double Mu = Total_tasks * (pow(2, 1./Total_tasks) - 1);
    int i = 0;
    for(i = 0; i < Total_tasks; i++)
    {
        U += ((1.0 * all_Tasks[i].C) / all_Tasks[i].P);
    }

    fputs("\\section{Pruebas de calendarizabilidad}\n", fp);

    if(flagRM)
    {
        fprintf(fp,
            "\\begin{frame}{Calendarizabilidad - RM}\n"
            "    \\begin{equation} \\label{eq:ej1}\n"
            "        U\n"
            "        =\n"
            "        \\sum_{i=1}^{n} \\frac{C_i}{T_i}\n"
            "        \\leq\n"
            "        n(2^{\\frac{1}{n}} - 1)\n"
            "        \\qquad\n"
            "        %.3f \\leq %.3f \\qquad \\therefore \n"
            "    \\end{equation}\n"
            "    \\begin{center}\n",
            U, Mu
        );
        fputs(U <= Mu ? "El conjunto es calendarizable\n" : 
            U < 1 ? "No hay certeza de la calendarizabilidad, se requiere prueba manual\n" : "El conjunto no es calendarizable\n",fp);
        fputs(
            "    \\end{center}\n"
            "    \\begin{equation} \\label{eq:ej2}\n"
            "        \\prod_{i=1}^{n} {(\\frac{C_i}{T_i} + 1)}\n"
            "        \\leq\n"
            "        2\n"
            "    \\end{equation}\n"
            "\\end{frame}\n\n",
            fp
        );
    }

    if(flagEDF)
    {
        fprintf(fp,
            "\\begin{frame}{Calendarizabilidad - EDF}\n"
            "    \\begin{equation} \\label{eq:ej3}\n"
            "        U\n"
            "        =\n"
            "        \\sum_{i=1}^{n} \\frac{C_i}{T_i}\n"
            "        <\n"
            "        1 \\qquad\n"// a little spacing
            "        %.3f < 1 \\qquad \\therefore \n"
            "    \\end{equation}\n"
            "    \\begin{center}\n", U
        );
        fputs(U < 1 ? "El conjunto es calendarizable\n" : "El conjunto no es calendarizable\n", fp);
        fputs("    \\end{center}\n \\end{frame}\n\n", fp);
    }
}

// parameter data to be used for determining unused processor times
void PrintTableHeader(FILE* fp, int startIdx, int* data, int status[2], int alg)
{
    int i = 0;
    for(i = 0; i < MAX_TIME_COLS; i++)
    {
        if( (status[0] != 0 && (i + startIdx) > status[1])//a deadline was not honored, scheduler execution halted
            || ((i + startIdx) > gMCM) //we have reached the MCM
          )
            fputs("|a", fp);
        else
            fprintf(fp, "|%s", data[i + startIdx] == 0 ? "a" : "c");
    }
}

void PrintTableTimes(FILE* fp, int startIdx)
{
    int i = 0;
    for(i = 0; i < MAX_TIME_COLS; i++)
    {
        fprintf(fp, "&%d", startIdx+i+1);
    }
}

void PrintTableTasks(FILE* fp, int startIdx, int* data, int status[2], int alg)
{
    int i = 0, j = 0;
    int currTime = 0;

    for(i = 0; i < Total_tasks; i++)
    {
        fprintf(fp, "            T%d", i+1);
        for(j = 0; j < MAX_TIME_COLS; j++)
        {
            currTime = startIdx+j+1;
            if( (status[0] != 0 && (j + startIdx) > status[1])//a deadline was not honored, scheduler execution halted
                || ((j + startIdx) > gMCM) //we have reached the MCM
              )
                fputs("& ", fp);
            else if( currTime % all_Tasks[i].P == 0)//mark deadline
            {
                if( data[startIdx+j] == (i+1) )//current task was scheduled
                    fprintf(fp, "&\\mcb{%s}", gCellColors[i]);
                else if(data[startIdx+j] == 0)//CPU subutilization
                    fprintf(fp, "&\\mcb{%s}", gCellColors[MAX_TASKS]);
                else
                    fputs("&\\mcb{}", fp);
            }
            else
            {
                if( data[startIdx+j] == (i+1) )//current task was scheduled
                    fprintf(fp, "&%s", gCellColors[i]);
                else
                    fputs("& ", fp);
            }
            //fprintf(fp, "&%d", startIdx+j+1);

        }
        fputs("\\\\ \n", fp);
    }
}

void SchedAlgorithmsSimSection(FILE* fp, int (*status)[2], int* RMData, int* EDFData, int* LLFData)
{
    fputs("\\section{Simulación de ejecución}\n", fp);
    int bSelected[3] = {flagRM, flagEDF, flagLLF};
    int* pData[3] = {RMData, EDFData, LLFData};
    char* pNames [3] = {"RM", "EDF", "LLF"};

    int i = 0, k = 0;
    int limit = 0, tCnt = 0;

    for(i = 0; i < 3; i++) // Iterate over all three algoritms
    {
        if(!bSelected[i]) continue;

        limit = status[i][0] == 0 ? gMCM : (status[i][1] + 1);
        limit = limit < MAX_TIME_COLS ? MAX_TIME_COLS : limit;
        if(flagSingleOutput)
        {
            tCnt = (limit / MAX_TIME_COLS) + (limit % MAX_TIME_COLS ? 1 : 0); // amount of tables to print
            printf("%s  %d table(s), limit: %d \n", pNames[i], tCnt, limit);
            fprintf(fp, "\\begin{frame}[shrink]{Simulación - %s}\n", pNames[i]);
            fputs("    \\begin{center}\n", fp);
            for(k = 0; k < tCnt; k++)
            {
                fputs("    \\begin{table}\n", fp);
                fputs("        \\begin{tabular}{ c", fp);
                PrintTableHeader(fp, k * MAX_TIME_COLS, pData[i], status[i], i);
                fputs(" }\n"
                    "            \\hline\n"
                    "            \\rowcolor{LightCyan}\n"
                    "            Tareas", fp);
                PrintTableTimes(fp, k * MAX_TIME_COLS);
                fputs(" \\\\ \n            \\hline\n", fp);
                PrintTableTasks(fp, k * MAX_TIME_COLS, pData[i], status[i], i);
                fputs("            \\hline\n", fp);
                fputs("        \\end{tabular}\n", fp);
                fputs("    \\end{table}\n", fp);

                if(k > 0 && k % MAX_TABLES_PER_SLIDE == 0)
                {
                    fputs("    \\end{center}\n", fp);
                    fputs("\\end{frame}\n\n", fp);
                    fprintf(fp, "\\begin{frame}{Simulación - %s}\n", pNames[i]);
                    fputs("    \\begin{center}\n", fp);
                }
            }

            fputs("    \\end{center}\n", fp);
            fputs("\\end{frame}\n\n", fp);
        }
        else
        {

        }

    }
}

void HeaderSection(FILE* fp)
{
    char* header = "\\documentclass{beamer}\n"
                    "\\usepackage[utf8]{inputenc}\n"
                    "\\usepackage[spanish]{babel}\n"
                    "\\usepackage{amsmath}\n"
                    "\\usepackage{color}\n"
                    "\\usepackage{xcolor,colortbl}\n\n"
                    "\\newcommand{\\mc}[2]{\\multicolumn{#1}{c}{#2}}\n"
                    "\\newcommand{\\mcb}[1]{\\multicolumn{1}{c!{\\vrule width 2pt}}{#1}}\n"

                    "\\definecolor{Gray}{gray}{0.85}\n"
                    "\\definecolor{LightCyan}{rgb}{0.88,1,1}\n\n"

                    "\\newcolumntype{a}{>{\\columncolor{Gray}}c}\n"
                    "\\newcolumntype{b}{>{\\columncolor{white}}c}\n\n"

                    "\\mode<presentation> {\n"
                    "    \\usetheme{Antibes}\n"
                    "    \\usecolortheme{whale}\n"
                    "}\n\n"
                    "\\usepackage{booktabs} %% Allows the use of \\toprule, \\midrule and \\bottomrule in tables\n"
                    ;
    char* title = "\\title{\\textbf{Titulo}}\n"//OOOOOOOOOOOOOJJJJJJOOOOOOOOOOOOOOOOOO
                  "\\author{\n"
                  "    Luis Arias Gómez \\\\ \n"
                  "    Guillermo López Navarro \\\\ \n"
                  "    \\textit{Instituto Tecnológico de Costa Rica}\\\\ \n"
                  "    Sistemas Operativos en Tiempo Real \\\\ \n"
                  "    \\tt III Cuatrimestre 2019\n"
                  "}\n"
                  "\\date{\\today}\n"
                  "\\titlegraphic{\\includegraphics[width=100pt,height=50pt]{../figs/Logo-tec.png}}\n"
                  ;

    fputs(header, fp);
    fputs(title, fp);
}