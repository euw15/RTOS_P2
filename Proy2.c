#include <stdio.h>
#include <gtk/gtk.h>

/*--------------------------Define Section-----------------------------*/
#define MAX_TASKS 6

/*--------------------------Enum and structs-----------------------------*/

enum algthms
{
  RM = 0, 
  EDF,
  LLF     
};

struct task_widget
{
  GtkWidget* lbl_C;
  GtkWidget* lbl_P;
};

struct task_Info
{
  int C;
  int P;
  int actualC;
  int active;
};

/*--------------------------Global Variables-----------------------------*/

// General
int Total_tasks;
struct task_Info all_Tasks[MAX_TASKS];
int gMCM;


// For GUI
struct task_widget all_Widgets[MAX_TASKS];
GtkWidget *mainWindow;

GtkWidget *spinC;
GtkWidget *spinP;

GtkWidget *chbtnRM;
GtkWidget *chbtnEDF;
GtkWidget *chbtnLLF;

GtkWidget *lblMCM;

GtkWidget *rdbtnSingle;
GtkWidget *rdbtnMultiple;

gboolean flagRM;
gboolean flagEDF;
gboolean flagLLF;
gboolean flagSingleOutput;

/*--------------------------------Beamer----------------------------------*/
#include "BeamerGen.h" //here because it requires what was defined above

void WriteTex(int (*status)[2], int* RMData, int* EDFData, int* LLFData)
{
    FILE *fp;

    fp = fopen("./RTOS_P2.tex", "w");
    //fprintf(fp, "This is testing for fprintf...\n");
    //fputs("This is testing for fputs...\n", fp);

    HeaderSection(fp);

    StartPresentation(fp);

    ParametersSection(fp);

    SchedAlgorithmsSection(fp);

    SchedAlgorithmsTestSection(fp);

    SchedAlgorithmsSimSection(fp, status, RMData, EDFData, LLFData);

    ClosePresentation(fp);

    fclose(fp);
}

/*--------------------------------Sched Algorithms----------------------------------*/

int getNextTask_RM()
{
  int next = -1;
  int actual_deadline = 0;
  int new_deadline = 0;
  int i = 0;
  for(i = 0; i < Total_tasks; i++)
  {
    if(all_Tasks[i].active == 1)
    {
      return i + 1;
    }
  }
  return next + 1;
}

int getNextTask_EDF(int time)
{
  int next = -1;
  int actual_deadline = 0;
  int new_deadline = 0;
  int i = 0;
  for(i = 0; i<Total_tasks; i++)
  {
    if(all_Tasks[i].active == 1)
    {
      if(next == -1)
      {
        next = i;
        actual_deadline = all_Tasks[i].P - (time%all_Tasks[i].P); 
      }
      else
      {
        new_deadline = all_Tasks[i].P - (time%all_Tasks[i].P); 
        if(new_deadline < actual_deadline)
        {
          actual_deadline = new_deadline;
          next = i;
        }
      }
    }
  }
  return next + 1;
}

int getNextTask_LLF(int time, int maxInd)
{
    //Li = Di - (t + [remaining C time])
    int next = -1;
    int i = 0, currLaxity = 0;
    int minLaxity = maxInd;
    for(i = 0; i<Total_tasks; i++)
    {
        if(all_Tasks[i].active == 1)
        {
            //                   absolute deadline for curr task         currTime          remaining C time
            currLaxity = (all_Tasks[i].P * (time / all_Tasks[i].P + 1)) - (time + (all_Tasks[i].C - all_Tasks[i].actualC) );
            if(currLaxity < minLaxity)
            {
                minLaxity = currLaxity;
                next = i;
            }
        }
    }
  return next + 1;
}


/*-----------------------------Scheduler----------------------------------*/

int updateNewTasks(int time)
{
    int i = 0;
    for(i = 0; i<Total_tasks; i++)
    {
      if(time%all_Tasks[i].P == 0) // Task enter to Ready List
      {
        if(all_Tasks[i].active == 1) // Task hasn't finished, did not meet deadline.
        {
          return i+1;
        }
        else // Task is activated.
        {
          all_Tasks[i].active = 1;
          all_Tasks[i].actualC = 0;
        }
      }
    }
  
  return 0;
}


int executeScheduler(int maxInd, int* taskResult, int actualAlgrthm, int* failTime )
{
  int i = 0;
  int nextTask = 0;
  int status = 0;

  //Initialize all tasks
  for(i = 0; i<Total_tasks; i++)
  {
    all_Tasks[i].actualC = 0;
    all_Tasks[i].active = 0;
  }

  //Check for all necessary times
  for(i = 0; i < maxInd; i++)
  {
    //Check for new tasks and break if any did not meet deadline
    status = updateNewTasks(i);
    if(status != 0)
    {
      printf("\nTarea %d no cumplió deadline. T = %d\n", status, *failTime);
      *failTime = i;
      break;
    }

    // Calculate next task to schedule
    switch(actualAlgrthm)
    {
      case LLF:
        nextTask = getNextTask_LLF(i, maxInd);
        break;
      case EDF:
        nextTask = getNextTask_EDF(i);
        break;
      case RM:
      default:
        nextTask = getNextTask_RM();
        break;
    }

    // Schedule selected task
    // Take it out of ready list if computation time is finished
    taskResult[i] = nextTask;
    printf("%d -> ", nextTask);
    all_Tasks[nextTask-1].actualC++;
    if(all_Tasks[nextTask-1].actualC == all_Tasks[nextTask-1].C)
      all_Tasks[nextTask-1].active = 0;
  }
  if(i == maxInd){
    status = updateNewTasks(i);
    if(status != 0)
    {
      printf("\nTarea %d no cumplió deadline. T = %d\n", status, *failTime);
      *failTime = i;
    }
  }
  return status;
}

/*--------------------------------MCM----------------------------------*/

int min(int a, int b)
{
  return a > b ? b : a;
}

int calcMCM(int a, int b)
{
  int minNum = min(a, b);
  int mcm = 0;
  for(int i = minNum; i > 0; i--)
  {
    if(a%i == 0 && b%i == 0)
    {
      mcm = a*b/i;
      break;
    }
  }
  return mcm;
}


int getMCM()
{
  if (Total_tasks == 0) return 0;
  int mcm = all_Tasks[0].P;
  for(int i = 1; i < Total_tasks; i++)
  {
    mcm = calcMCM(mcm, all_Tasks[i].P);
  }
  return mcm;
}

void updateMCM()
{
  int MCM = getMCM();
  char c[20];
  sprintf(c, "%d\n", MCM);
  gtk_label_set_text(GTK_LABEL(lblMCM), c);
}

/*--------------------------------GUI----------------------------------*/

void promptMsg(GtkMessageType type, const char* msg){
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new (GTK_WINDOW(mainWindow),
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                 type,
                                 GTK_BUTTONS_CLOSE,
                                 "%s", msg);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void ExecuteEverything()
{
    flagRM = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (chbtnRM));
    flagEDF = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (chbtnEDF));
    flagLLF = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (chbtnLLF));
    flagSingleOutput = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (rdbtnSingle));
  if(Total_tasks == 0)
  {
    promptMsg(GTK_MESSAGE_ERROR, "Error: At least one task needed.");
  }
  else if(!flagRM && !flagEDF && !flagLLF)
  {
    promptMsg(GTK_MESSAGE_ERROR, "Error: Check at least one scheduler.");
  }
  else
  {
    int mcm = gMCM = getMCM();
    printf("MCM: %d\n", mcm);

    int* schedRM = (int *) calloc(mcm, sizeof(int));
    int* schedEDF = (int *) calloc(mcm, sizeof(int));
    int* schedLLF = (int *) calloc(mcm, sizeof(int));

    int status[3][2];// {task, time}
    int time = 0;

    if(flagRM) 
    {
      printf("Ejecutando RM\n");
      status[0][0] = executeScheduler(mcm, schedRM, RM, &time);
      status[0][1] = time;
      printf("\nSe ha ejecutado EDF\n");
    }
    
    if(flagEDF) 
    {
      printf("Ejecutando EDF\n");
      status[1][0] = executeScheduler(mcm, schedEDF, EDF, &time);
      status[1][1] = time;
      printf("\nSe ha ejecutado EDF\n");
    }

    if(flagLLF) 
    {
      printf("Ejecutando LLF\n");
      status[2][0] = executeScheduler(mcm, schedLLF, LLF, &time);
      status[2][1] = time;
      printf("\nSe ha ejecutado LLF\n");
    }

    // Generate .tex and .pdf
    WriteTex(status, schedRM, schedEDF, schedLLF);
    system("cd ltx && pdflatex ../RTOS_P2.tex -quiet > /dev/null 2>&1 && mv -f RTOS_P2.pdf ../");
    printf("Se ha generado el reporte PDF\n");

  
    //ERROR!!!
  }

}

void addTask()
{
  char c[20];
  int newC = 0;
  int newP = 0;
  if(Total_tasks < MAX_TASKS)
  {
    newC = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinC));
    newP = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinP));

    if(newC <= newP)
    {
      int i = 0;

      // Sort new task
      for(i = Total_tasks; i > 0; i--)
      {
        if(newP < all_Tasks[i-1].P)
        {
          all_Tasks[i].C = all_Tasks[i-1].C;
          all_Tasks[i].P = all_Tasks[i-1].P;
        }
        else
        {          
          break;
        }
      }
      all_Tasks[i].C = newC;
      all_Tasks[i].P = newP;

      Total_tasks++;

      // UpdateGui
      for(i = 0; i < Total_tasks; i++)
      {
        sprintf(c, "%d", all_Tasks[i].C);
        gtk_label_set_text(GTK_LABEL(all_Widgets[i].lbl_C), c);
  
        sprintf(c, "%d", all_Tasks[i].P);
        gtk_label_set_text(GTK_LABEL(all_Widgets[i].lbl_P), c);
      }
      updateMCM();


    }
    else
    {
      promptMsg(GTK_MESSAGE_ERROR, "Error: The computation time can not be greater than the period.");
      //ERROR!!!
    }  
  }
  else
  {
    promptMsg(GTK_MESSAGE_INFO, "6 is maximum number of tasks.");
    //ERROR!!!
  }  
}

void deleteTask()
{
  if(Total_tasks > 0)
  {
    Total_tasks--;
    gtk_label_set_text(GTK_LABEL(all_Widgets[Total_tasks].lbl_C), "");
    gtk_label_set_text(GTK_LABEL(all_Widgets[Total_tasks].lbl_P), "");
    all_Tasks[Total_tasks].C = 0;
    all_Tasks[Total_tasks].P = 0;
    updateMCM();
  }
  else
  {
    //ERROR!!!
  }  
}

void resetTasks()
{
  for(int i = 0; i < Total_tasks; i++)
  { 
    gtk_label_set_text(GTK_LABEL(all_Widgets[i].lbl_C), "");
    gtk_label_set_text(GTK_LABEL(all_Widgets[i].lbl_P), "");
    all_Tasks[Total_tasks].C = 0;
    all_Tasks[Total_tasks].P = 0;
  }
  Total_tasks=0;
  updateMCM();

}

void set_GUI()
{
  GtkWidget *boxMain;
  GtkWidget *boxNewTask;
  GtkWidget *boxNewTaskInfo;
  GtkWidget *boxNewTaskButtons;
  GtkWidget *boxMCM;
  GtkWidget *boxOptions;
  GtkWidget *boxSchedulers;
  GtkWidget *boxOutput;
  GtkWidget *boxRun;


  GtkWidget *lblInfo;
  GtkWidget *lblC;
  GtkWidget *lblP;
  GtkWidget *inC;
  GtkWidget *inP;
  GtkAdjustment *spinnerC;
  GtkAdjustment *spinnerP;

  GtkWidget *btnNew;
  GtkWidget *btnReset;
  GtkWidget *btnDelete;
  GtkWidget *btnRun;


  GtkWidget *frmNewTask;
  GtkWidget *frmTasks;
  GtkWidget *frmMCM;
  GtkWidget *frmSchedulers;
  GtkWidget *frmOutput;

  GtkWidget *table;
  GtkWidget *label;

  gtk_init(NULL, NULL);
  
  //Initialize Main Window
  mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(mainWindow), "RTOS");
  gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);

  /*********** First Frame **********************/

  // Create box with New Tasks Info
  boxNewTaskInfo = gtk_hbox_new (FALSE,0);

  lblInfo = gtk_label_new("New Task Info: ");
  lblC = gtk_label_new("C: ");
  inC = gtk_entry_new();
  spinnerC = (GtkAdjustment *) gtk_adjustment_new(1.0, 1.0, 1000.0, 1.0, 5.0, 0);
  spinC = gtk_spin_button_new (spinnerC, 1,0);
  lblP = gtk_label_new("P: ");
  inP = gtk_entry_new();
  spinnerP = (GtkAdjustment *) gtk_adjustment_new(1.0, 1.0, 1000.0, 1.0, 5.0, 0);
  spinP = gtk_spin_button_new (spinnerP, 1,0);
  gtk_box_pack_start (GTK_BOX(boxNewTaskInfo), lblInfo, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTaskInfo), lblC, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTaskInfo), spinC, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTaskInfo), lblP, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTaskInfo), spinP, TRUE, TRUE, 10);

  // Create box with New Tasks Buttons
  boxNewTaskButtons = gtk_hbox_new (FALSE,0);
  btnNew = gtk_button_new_with_label ("Add Task");
  gtk_signal_connect (GTK_OBJECT (btnNew), "clicked", GTK_SIGNAL_FUNC (addTask), (gpointer) "");
  btnDelete =  gtk_button_new_with_label ("Delete Last Task");
  gtk_signal_connect (GTK_OBJECT (btnDelete), "clicked", GTK_SIGNAL_FUNC (deleteTask), (gpointer) "");
  btnReset = gtk_button_new_with_label ("Reset Tasks");
  gtk_signal_connect (GTK_OBJECT (btnReset), "clicked", GTK_SIGNAL_FUNC (resetTasks), (gpointer) "");
  gtk_box_pack_start (GTK_BOX(boxNewTaskButtons), btnNew, TRUE, FALSE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTaskButtons), btnDelete, TRUE, FALSE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTaskButtons), btnReset, TRUE, FALSE, 10);

  // Create Box that contains new tasks boxes
  boxNewTask = gtk_vbox_new (TRUE,0);
  gtk_box_pack_start (GTK_BOX(boxNewTask), boxNewTaskInfo, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxNewTask), boxNewTaskButtons, FALSE, FALSE, 10);

  // Create first frame: New Task
  frmNewTask = gtk_frame_new ("New Task");
  gtk_container_add (GTK_CONTAINER (frmNewTask), boxNewTask);

  /*********** Second Frame **********************/

  // Create Table 
  table = gtk_table_new (7, 3, TRUE);
  label = gtk_label_new("# Task");
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
  label = gtk_label_new("C");
  gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 0, 1);
  label = gtk_label_new("P");
  gtk_table_attach_defaults (GTK_TABLE (table), label, 2, 3, 0, 1);
  char c[20];
  for(int i = 1; i<=MAX_TASKS; i++)
  { 
    sprintf(c, "%d", i);
    label = gtk_label_new(c);
    gtk_table_attach_defaults (GTK_TABLE(table), label, 0, 1, i, i+1);

    all_Widgets[i-1].lbl_C = gtk_label_new("");
    gtk_table_attach_defaults (GTK_TABLE(table), all_Widgets[i-1].lbl_C, 1, 2, i, i+1);
    all_Widgets[i-1].lbl_P = gtk_label_new("");
    gtk_table_attach_defaults (GTK_TABLE(table), all_Widgets[i-1].lbl_P, 2, 3, i, i+1);
  }

  // Create Second frame: All Task
  frmTasks = gtk_frame_new ("All Tasks");
  gtk_container_add (GTK_CONTAINER (frmTasks), table);

  /*********** Thirdh Frame **********************/

  boxMCM = gtk_hbox_new (FALSE,0);
  lblMCM = gtk_label_new("0\n");
  updateMCM();
  gtk_box_pack_start (GTK_BOX(boxMCM), lblMCM, TRUE, FALSE, 50);

  // Create Thirdh frame: MCM
  frmMCM= gtk_frame_new ("MCM");
  gtk_container_add (GTK_CONTAINER (frmMCM), boxMCM);

  /*********** Fourth Frame **********************/

  boxOutput= gtk_vbox_new (FALSE,0);
  rdbtnSingle = gtk_radio_button_new_with_label (NULL, "One algorithm at a time ");
  rdbtnMultiple = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rdbtnSingle), "Multiple algoritms per Slide");
  gtk_box_pack_start (GTK_BOX(boxOutput), rdbtnSingle, FALSE, FALSE, 10);
  gtk_box_pack_start (GTK_BOX(boxOutput), rdbtnMultiple, TRUE, FALSE, 10);

  frmOutput = gtk_frame_new ("Output");
  gtk_container_add (GTK_CONTAINER (frmOutput), boxOutput);

  boxSchedulers = gtk_vbox_new (FALSE,0);
  chbtnRM = gtk_check_button_new_with_label("RM: Rate-Monotonic");
  chbtnEDF = gtk_check_button_new_with_label("EDF: Earliest Deadline First");
  chbtnLLF = gtk_check_button_new_with_label("LLF: Least Laxity First");
  gtk_box_pack_start (GTK_BOX(boxSchedulers), chbtnRM, FALSE, FALSE, 10);
  gtk_box_pack_start (GTK_BOX(boxSchedulers), chbtnEDF, TRUE, FALSE, 10);
  gtk_box_pack_start (GTK_BOX(boxSchedulers), chbtnLLF, TRUE, FALSE, 10);

  frmSchedulers = gtk_frame_new ("Schedulers");
  gtk_container_add (GTK_CONTAINER (frmSchedulers), boxSchedulers);

  boxOptions = gtk_hbox_new (FALSE,0);
  gtk_box_pack_start (GTK_BOX(boxOptions), frmSchedulers, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxOptions), frmOutput, TRUE, TRUE, 10);


  /*********** Fifth Frame **********************/
  boxRun = gtk_hbox_new (FALSE,0);
  btnRun = gtk_button_new_with_label ("Run");
  gtk_signal_connect (GTK_OBJECT (btnRun), "clicked", GTK_SIGNAL_FUNC (ExecuteEverything), (gpointer) "");
  gtk_box_pack_start (GTK_BOX(boxRun), btnRun, TRUE, FALSE, 10);


  /*********** Main Window **********************/
  boxMain = gtk_vbox_new (FALSE,0);
  gtk_box_pack_start (GTK_BOX(boxMain), frmNewTask, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxMain), frmTasks, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxMain), frmMCM, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxMain), boxOptions, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX(boxMain), boxRun, TRUE, TRUE, 10);
  gtk_container_add (GTK_CONTAINER (mainWindow), boxMain);
  

  g_signal_connect (mainWindow, "delete-event", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (mainWindow, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  //g_timeout_add(10, update_GUI, table);

  gtk_container_set_border_width (GTK_CONTAINER (mainWindow), 10);

  gtk_widget_show_all(mainWindow);
    
  gtk_main ();
}
/*--------------------------------GUI-END------------------------------*/

int main(int argc, char *argv[])
{
  Total_tasks = 0;
  for(int i = 0; i < MAX_TASKS; i++)
  { 
    all_Tasks[Total_tasks].C = 0;
    all_Tasks[Total_tasks].P = 0;
  }
  int i = 0%12;
  printf("%d\n", i);
  //gtk_fixed_put(GTK_FIXED(fixed), btn3, 100, 100);
  //gtk_widget_set_size_request(btn3, 80, 30);


  set_GUI();
}