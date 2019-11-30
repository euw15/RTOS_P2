#include <stdio.h>
#include <math.h>

#define MAX_TIME_COLS 10
#define MAX_TABLES_PER_SLIDE 4

#define MY_MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MY_MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

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

float gTaskCntPadding[MAX_TASKS] = 
{
    1.1, 1.1, 1.1, 1.2, 1.37, 1.55
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
void PrintTableHeader(FILE* fp, int startIdx, int* data, int dTime, int dTask)
{
    int i = 0;
    for(i = 0; i < MAX_TIME_COLS; i++)
    {
        if( (dTask != 0 && (i + startIdx) > dTime)//a deadline was not honored, scheduler execution halted
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

void PrintTableTasks(FILE* fp, int startIdx, int* data, int dTime, int dTask)
{
    int i = 0, j = 0;
    int currTime = 0;

    for(i = 0; i < Total_tasks; i++)
    {
        fprintf(fp, "            T%d", i+1);
        for(j = 0; j < MAX_TIME_COLS; j++)
        {
            currTime = startIdx+j+1;
            if( (dTask != 0 && (j + startIdx) >= dTime)//a deadline was not honored, scheduler execution halted
                || ((j + startIdx) > gMCM) //we have reached the MCM
              )
            {
                if (dTask == (i + 1))//this was the task that missed its deadline
                    fputs("&\\cellcolor{red}", fp);
                else
                    fputs("& ", fp);
            }
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
    int limits[3] = { // Obtain greatest time to display for each algorithm, set zero for those not selected to run
        (!bSelected[0] ? 0 : status[0][0] == 0 ? gMCM : (status[0][1] + 1)),
        (!bSelected[1] ? 0 : status[1][0] == 0 ? gMCM : (status[1][1] + 1)),
        (!bSelected[2] ? 0 : status[2][0] == 0 ? gMCM : (status[2][1] + 1)),
    };

    int i = 0, k = 0;
    int limit = 0, tCnt = 0;
        
    if(flagSingleOutput)
    {
        for(i = 0; i < 3; i++) // Iterate over all three algoritms
        {
            if(!bSelected[i]) continue;

            limit = status[i][0] == 0 ? gMCM : (status[i][1] + 1);
            limit = MY_MIN(limit, MAX_TIME_COLS);

            tCnt = (limit / MAX_TIME_COLS) + (limit % MAX_TIME_COLS ? 1 : 0); // amount of tables to print
            printf("%s  %d table(s), limit: %d \n", pNames[i], tCnt, limit);
            fprintf(fp, "\\begin{frame}[shrink]{Simulación - %s (MCM: %d)}\n", pNames[i], gMCM);
            fputs("    \\begin{center}\n", fp);
            for(k = 0; k < tCnt; k++)
            {
                fputs("    \\begin{table}\n", fp);
                fputs("        \\begin{tabular}{ c", fp);
                PrintTableHeader(fp, k * MAX_TIME_COLS, pData[i], status[i][1], status[i][0]);
                fputs(" }\n"
                    "            \\hline\n"
                    "            \\rowcolor{LightCyan}\n"
                    "            Tareas", fp);
                PrintTableTimes(fp, k * MAX_TIME_COLS);
                fputs(" \\\\ \n            \\hline\n", fp);
                PrintTableTasks(fp, k * MAX_TIME_COLS, pData[i], status[i][1], status[i][0]);
                fputs("            \\hline\n", fp);
                fputs("        \\end{tabular}\n", fp);
                fputs("    \\end{table}\n", fp);

                if(k > 0 && k % MAX_TABLES_PER_SLIDE == 0)
                {
                    fputs("    \\end{center}\n", fp);
                    fputs("\\end{frame}\n\n", fp);
                    fprintf(fp, "\\begin{frame}[shrink]{Simulación - %s (MCM: %d)}\n", pNames[i], gMCM);
                    fputs("    \\begin{center}\n", fp);
                }
            }

            if(status[i][0] != 0) 
                fprintf(fp, "    La tarea T%d ha perdido su deadline en el tiempo %d\n", status[i][0], status[i][1]);
            fputs("    \\end{center}\n", fp);
            fputs("\\end{frame}\n\n", fp);
        }
    }
    else // Multiple algorithms per slide
    {
        k = 0; // For keeping track of how many "cpu times" have we printed so far
        tCnt = 0; // For keeping track of how many slides have we printed so far

        while(k < limits[0] || k < limits[1] || k < limits[2] ) //keep adding frames while we have pending task schedules
        {
            tCnt = k / MAX_TIME_COLS;
            fprintf(fp, "\\begin{frame}[shrink=%d]{Simulación de ejecución (MCM: %d)}\n", MY_MAX(10*Total_tasks, 50), gMCM);
            fprintf(fp, "\\begin{minipage}[c][%.2f\\paperheight][c]{\\textwidth}", gTaskCntPadding[Total_tasks-1]);
            fputs("    \\begin{center}\n", fp);

            for(i = 0; i < 3; i++) // Iterate over all three algoritms
            {
                if(bSelected[i])//
                {
                    fprintf(fp, "    %s \\\\ \n", pNames[i]);
                    fputs("    \\begin{table}\n", fp);
                    fputs("        \\begin{tabular}{ c", fp);
                    PrintTableHeader(fp, tCnt * MAX_TIME_COLS, pData[i], status[i][1], status[i][0]);
                    fputs(" }\n"
                        "            \\hline\n"
                        "            \\rowcolor{LightCyan}\n"
                        "            Tareas", fp);
                    PrintTableTimes(fp, tCnt * MAX_TIME_COLS);
                    fputs(" \\\\ \n            \\hline\n", fp);
                    PrintTableTasks(fp, tCnt * MAX_TIME_COLS, pData[i], status[i][1], status[i][0]);
                    fputs("            \\hline\n", fp);
                    fputs("        \\end{tabular}\n", fp);
                    fputs("    \\end{table}\n", fp);
                    if( (k + MAX_TIME_COLS) > limits[i] && status[i][0] != 0) 
                        fprintf(fp, "    La tarea T%d ha perdido su deadline en el tiempo %d \\\\ \n", status[i][0], status[i][1]);
                }
            }
            fputs("    \\end{center}\n", fp);
            fputs("\\end{minipage}\n\n", fp);
            fputs("\\end{frame}\n\n", fp);

            k += MAX_TIME_COLS;
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
    char* title = "\\title{\\textbf{Proyecto 2}}\n"
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