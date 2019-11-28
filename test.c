#include <stdio.h>

struct task_Info
{
  int C;
  int P;
  int actualC;
  int active;
};

struct task_Info all_Tasks[6];

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

//void ParametersSection(FILE* fp, gboolean rm, gboolean edf, gboolean llf, struct task_Info* ti, int taskCnt)
void ParametersSection(FILE* fp, int rm, int edf, int llf, struct task_Info* ti, int taskCnt)
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
    //------------------------remove----------------->
        "                        T1 & a & b \\\\ \n"
        "                        T2 & a & b \\\\ \n"
        "                        \\hline\n"
    //------------------------remove-----------------<
        "                    \\end{tabular}\n"
        "                \\end{table}\n\n\n"
        "            \\column{30mm}\n"
        "                Algoritmos seleccionados:\n"
        "                \\begin{enumerate}\n"
    //------------------------remove----------------->
        "                   \\item RM\n"
        "                   \\item EDF\n"
        "                   \\item LLF\n"
    //------------------------remove-----------------<
        ;
    char* string3 =
        "                \\end{enumerate}\n"
        "        \\end{columns}\n"
        "    \\end{block}\n"
        "\\end{frame}\n\n"
        ;
    fputs(string1, fp);
    for( i = 0; i < taskCnt; i++)
    {
        fprintf(fp, "                        T%d & %d & %d \\\\ \n", i+1, ti[i].C, ti[i].P);
        fputs("                        \\hline\n", fp);
    }
    fputs(string2, fp);
    if(rm)
        fputs("                   \\item RM\n", fp);
    if(edf)
        fputs("                   \\item EDF\n", fp);
    if(llf)
        fputs("                   \\item LLF\n", fp);
    fputs(string3, fp);
}

void SchedAlgorithmsSection(FILE* fp)
{
    char* string =
    "\\section{Algoritmos de calendarización}"
    "\\begin{frame}{Rate Monotonic}"
    "    Algoritmo de calendarización de prioridad estática. La prioridad de cada tarea está dada según su período. A menor período, mayor prioridad.\\\\~\\\\"
        
    "    Prueba de calendarizabilidad:"

    "    \\begin{equation} \\label{eq:ej1}"
    "        U"
    "        ="
    "        \\sum_{i=1}^{n} \\frac{C_i}{T_i}"
    "        \\leq"
    "        n(2^{\\frac{1}{n}} - 1)"
    "    \\end{equation}"
        
    "    \\begin{equation} \\label{eq:ej1}"
    "        \\prod_{i=1}^{n} {(\\frac{C_i}{T_i} + 1)}"
    "        \\leq"
    "        2"
    "    \\end{equation}"

    "\\end{frame}"

    "\\begin{frame}{Earliest Deadline First}"
    "    Algoritmo de calendarización de prioridad dinámica. La prioridad de cada tarea está dada según la proximidad de su deadline. En un momento dado, la tarea de mayor prioridad será aquella cuyo deadline esté más próximo a alcanzarse.\\\\~\\\\"
        
    "    Prueba de calendarizabilidad:"
    "    \\begin{center}"
//    "        \\includegraphics[]{../figs/EDF_Schedulability.JPG}"
    "    \\end{center}"
    "\\end{frame}"

    "\\begin{frame}{Least Laxity First}"
    "    Algoritmo de calendarización de prioridad dinámica. La prioridad de cada tarea está dada según la proximidad de su deadline. En un momento dado, la tarea de mayor prioridad será aquella cuyo deadline esté más próximo a alcanzarse. BLABLA\\\\~\\\\"
        

    "\\end{frame}"
    ;
    fputs(string, fp);
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

int main(int argc, char *argv[])
{
    FILE *fp;

    fp = fopen("./RTOS_P2.tex", "w");
    //fprintf(fp, "This is testing for fprintf...\n");
    //fputs("This is testing for fputs...\n", fp);

    HeaderSection(fp);

    StartPresentation(fp);

    ParametersSection(fp, 0, 0, 0, NULL, 0);

    SchedAlgorithmsSection(fp);

    ClosePresentation(fp);

    fclose(fp);
}