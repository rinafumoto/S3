#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int b,i,k,j, buf_count=0,n,pid,ed,sd,et,st,dd,du,dt,label=0,back,count=0,nacc=0,nrej=0,x,z;
    int fd[2];//parent->child
    int c[2];//child->parent
    int slot=56;
    char acceptsubcode[56][100];
    char lines[20];
    char buf[100],bat[100],filename[100],before_def[100],p[20],fname[100],status[20],startdate[8],q[20],enddate[8],starttime[4],endtime[4],line[100],duedate[20],duration[20],subcode[10],duetime[20],s[20],l[10];
    FILE *infile,*outfilep,*filetxt,*logfile,*summaryReport,*acceptsubject,*list;
	
	for (i=0; i<56; i++) {
		strcpy(acceptsubcode[i],"N/A");
	}//put N/A to the array
    outfilep=fopen("file.txt","w");//open a text file to store the inputs
    if (outfilep == NULL) {
        printf("Error in making compact file\n");
        exit(1);
    }//error handle
    printf("~~WELCOME TO S3~~\n");
    if (pipe(fd) < 0) {
        printf("Pipe creation(parent write) error\n");
        exit(1);
    }//make pipe
    if (pipe(c) < 0) {
        printf("Pipe creation(child write) error\n");
        exit(1);
    }//make pipe
    pid = fork();//make child
    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    }//error handle
    else if (pid==0){//child process
        int check[56];
        close(fd[1]);//close child out
        acceptsubject=fopen("acceptsubject.dat","w+");//open a data file to store the accepted requests
        while((n = read(fd[0],line,80)) > 0){
            strcpy(status, "accepted");
            line[n]=0;//read lines from pipe
            strcpy(q, strtok(line, " ")); 
            if(strcmp(q,"addPeriod")==0) {//get the first word from the input
                strcpy(startdate, strtok(NULL, " "));
                strcpy(enddate, strtok(NULL, " "));
                strcpy(starttime, strtok(NULL, " "));
                strcpy(endtime, strtok(NULL, " "));//split the input lines of file.txt
                ed=atoi(enddate);
                sd=atoi(startdate);
                et=atoi(endtime);
                st=atoi(starttime);//change the input to integer
                if(ed<sd||et<st||et%100!=0||st%100!=0){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }//error handle
                if (strcmp(status,"rejected")==0){
					nrej++;
		        }//count rejected requests
                if (strcmp(status,"accepted")==0){
					nacc++;
		        }//count accepted requests
                write(c[1],status,strlen(status));//send the status to parent
                memset(status,0,sizeof(status));//reset the status

            }else if (strcmp(q,"addAssignment")==0) {
                strcpy(subcode, strtok(NULL, " "));
                strcpy(duedate, strtok(NULL, " "));
                strcpy(duration, strtok(NULL, " "));//split the input lines of file.txt
                dd=atoi(duedate);
                du=atoi(duration);
                    
                if(dd<sd||dd>ed){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }
                else if(du>slot){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }//error handle
                else{
                    label=(dd-sd+1)*4;
                    i=0;
                    while(i<=label){
                        if(check[i]!=1){
                            du--;
                        }
                        i++;
                    }
                    if(du>0){
                        memset(status,0,sizeof(status));
                        strcpy(status, "rejected");
                    }//check if it can be accepted or rejected

                    du=atoi(duration);

                    if (strcmp(status,"rejected")!=0){
                        i=0;
                        while(i<=label&&du>=0){
                            if(check[i]!=1){
                                du--;
                                check[i]=1;
								strcpy(acceptsubcode[i],subcode);
                            }
                            i++;
                        }
                    }//add the request in the timetable
                }
                if (strcmp(status,"rejected")==0){
                    nrej++;
                }//count rejected requests
                if (strcmp(status,"accepted")==0){
                    nacc++;
                    du=atoi(duration);
                    for (x=1; x<=du;x++){
                        fprintf(acceptsubject, "%s\n",subcode);
                    }
                }//count accepted requests and add the request in the data file
                write(c[1],status,strlen(status));//send the status to parent
                memset(status,0,sizeof(status));//reset the status

            } else if (strcmp(q,"addProject")==0) {
                strcpy(subcode, strtok(NULL, " "));
                strcpy(duedate, strtok(NULL, " "));
                strcpy(duration, strtok(NULL, " "));//split the input lines of file.txt
                dd=atoi(duedate);
                du=atoi(duration);
                if(dd<sd||dd>ed){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }
                else if(du>slot){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }//error handle
                else{
                    label=(dd-sd+1)*4;
                    i=0;
                    while(i<=label){
                        if(check[i]!=1){
                            du--;
                        }
                        i++;
                    }
                    if(du>0){
                        memset(status,0,sizeof(status));
                        strcpy(status, "rejected");
                    }//check if it can be accepted or rejected
                    du=atoi(duration);
                    if (strcmp(status,"rejected")!=0){
                        i=0;
                        while(i<=label&&du>=0){
                            if(check[i]!=1){
                                du--;
                                check[i]=1;
								strcpy(acceptsubcode[i],subcode);
                            }
                            i++;
                        }
                    }//add the request in the timetable
                }
                
                if (strcmp(status,"rejected")==0){
					nrej++;
		        }//count rejected requests
                if (strcmp(status,"accepted")==0){
					nacc++;
                    du=atoi(duration);
                    for (x=1; x<=du;x++){
                        fprintf(acceptsubject, "%s\n",subcode);
                    }
		        }//count accepted requests and add the request in the data file
                write(c[1],status,strlen(status));//send the status to parent
                memset(status,0,sizeof(status));//reset the status
            } else if (strcmp(q,"addRevision")==0) {
                strcpy(subcode, strtok(NULL, " "));
                strcpy(duedate, strtok(NULL, " "));
                strcpy(duetime, strtok(NULL, " "));
                strcpy(duration, strtok(NULL, " "));//split the input lines of file.txt
                dd=atoi(duedate);
                dt=atoi(duetime);
                du=atoi(duration);

                if(dd<sd||dd>ed||dt<st||dt>et||dt%100!=0){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }
                else if(du>slot){
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }//error handle
                else{
                    label=(dd-sd)*4+(dt-st)/100;
                    for(i=0;i<du;i++){
                        if(check[label+i]==1){
                            memset(status,0,sizeof(status));
                            strcpy(status, "rejected");
                        }
                    }//check if it can be accepted or rejected
                    if (strcmp(status,"rejected")!=0){
                        for(i=0;i<du;i++){
                            check[label]=1;
							strcpy(acceptsubcode[label],subcode);
                            label++;
                        }
                    }//add the request in the timetable
                }
                if (strcmp(status,"rejected")==0){
					nrej++;
		        }//count rejected requests
                if (strcmp(status,"accepted")==0){
					nacc++;
                    du=atoi(duration);
                    for (x=1; x<=du;x++){
                        fprintf(acceptsubject, "%s\n",subcode);
                    }
		        }//count accepted requests and add the request in the data file
                write(c[1],status,strlen(status));//send the status to parent
                memset(status,0,sizeof(status));//reset the status
            } else if (strcmp(q,"addActivity")==0) {
                strcpy(subcode, strtok(NULL, " "));
                strcpy(duedate, strtok(NULL, " "));
                strcpy(duetime, strtok(NULL, " "));
                strcpy(duration, strtok(NULL, " "));//split the input lines of file.txt
                dd=atoi(duedate);
                dt=atoi(duetime);
                du=atoi(duration);

                if(dd<sd||dd>ed||dt<st||dt>et||dt%100!=0){
                   // printf("Please input correct time");
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }
                else if(du>slot){
                    //printf("Activity cannot be finished");
                    memset(status,0,sizeof(status));
                    strcpy(status, "rejected");
                }//error handle
                else{
                    label=(dd-sd)*4+(dt-st)/100;
                    for(i=0;i<du;i++){
                        if(check[label+i]==1){
                            memset(status,0,sizeof(status));
                            strcpy(status, "rejected");
                        }
                    }//check if it can be accepted or rejected
                    if (strcmp(status,"rejected")!=0){
                        for(i=0;i<du;i++){
                            check[label]=1;
							strcpy(acceptsubcode[label],subcode);
                            label++;
                        }
                    }//add the request in the timetable
                }
                if (strcmp(status,"rejected")==0){
					nrej++;
		        }//count rejected requests
                if (strcmp(status,"accepted")==0){
					nacc++;
                    du=atoi(duration);
                    for (x=1; x<=du;x++){
                        fprintf(acceptsubject, "%s\n",subcode);
                    }
		        }//count accepted requests and add the request in the data file
                write(c[1],status,strlen(status));//send the status to parent
                memset(status,0,sizeof(status));//reset the status
        } else if (strcmp(q,"summary")==0) {
			strcpy(status, "null");
			summaryReport=fopen(strtok(NULL," "),"w");//open summary report file
            if (summaryReport == NULL) {
                printf("Error in making Summary Report\n");
                exit(1);
            }//error handle
            fprintf(summaryReport,"***Summary Report***\n");
            fprintf(summaryReport,"Algorithm used:FCFS\n");
            fprintf(summaryReport, "**************************\n");
			fprintf(summaryReport,"There are %d requests\n",count);
            fprintf(summaryReport,"Number of request accepted: %d\n",nacc);
            fprintf(summaryReport,"Number of request rejected: %d\n",nrej);//print report
			label=0;
			for(i=0;i<56;i++){
				if(check[i]==1){
					label++;
				}
			}
            fclose(acceptsubject);
                    
			fprintf(summaryReport,"Number of slots used: %d (%5.2f%)\n",label, label/(float)slot*100.00);
            fprintf(summaryReport, "**************************\n");
            fprintf(summaryReport, "Date\t\t19:00\t\t20:00\t\t21:00\t\t22:00\n");
            fprintf(summaryReport, "2019-04-08%15s%15s%15s%15s\n",acceptsubcode[0],acceptsubcode[1],acceptsubcode[2],acceptsubcode[3]);
            fprintf(summaryReport, "2019-04-09%15s%15s%15s%15s\n",acceptsubcode[4],acceptsubcode[5],acceptsubcode[5],acceptsubcode[7]);
            fprintf(summaryReport, "2019-04-10%15s%15s%15s%15s\n",acceptsubcode[8],acceptsubcode[9],acceptsubcode[10],acceptsubcode[11]);
            fprintf(summaryReport, "2019-04-11%15s%15s%15s%15s\n",acceptsubcode[12],acceptsubcode[13],acceptsubcode[14],acceptsubcode[15]);
            fprintf(summaryReport, "2019-04-12%15s%15s%15s%15s\n",acceptsubcode[16],acceptsubcode[17],acceptsubcode[18],acceptsubcode[19]);
            fprintf(summaryReport, "2019-04-13%15s%15s%15s%15s\n",acceptsubcode[20],acceptsubcode[21],acceptsubcode[22],acceptsubcode[23]);
            fprintf(summaryReport, "2019-04-14%15s%15s%15s%15s\n",acceptsubcode[24],acceptsubcode[25],acceptsubcode[26],acceptsubcode[27]);
            fprintf(summaryReport, "2019-04-15%15s%15s%15s%15s\n",acceptsubcode[28],acceptsubcode[29],acceptsubcode[30],acceptsubcode[31]);
            fprintf(summaryReport, "2019-04-16%15s%15s%15s%15s\n",acceptsubcode[32],acceptsubcode[33],acceptsubcode[34],acceptsubcode[35]);
            fprintf(summaryReport, "2019-04-17%15s%15s%15s%15s\n",acceptsubcode[36],acceptsubcode[37],acceptsubcode[38],acceptsubcode[39]);
            fprintf(summaryReport, "2019-04-18%15s%15s%15s%15s\n",acceptsubcode[40],acceptsubcode[41],acceptsubcode[42],acceptsubcode[43]);
            fprintf(summaryReport, "2019-04-19%15s%15s%15s%15s\n",acceptsubcode[44],acceptsubcode[45],acceptsubcode[46],acceptsubcode[47]);
            fprintf(summaryReport, "2019-04-20%15s%15s%15s%15s\n",acceptsubcode[48],acceptsubcode[49],acceptsubcode[50],acceptsubcode[51]);
            fprintf(summaryReport, "2019-04-21%15s%15s%15s%15s\n",acceptsubcode[52],acceptsubcode[53],acceptsubcode[54],acceptsubcode[55]);
            fclose(summaryReport);//print timetable
			count--;
		}
		count++;//count the number of requests
	}
    close(fd[0]);
    close(c[1]);//close pipes
    //sleep(3);
    }
    else{//parent process
        close(fd[0]);//close parent in
        close(c[1]);//close parent out
        printf("Please enter\n");
        while ((b = read(STDIN_FILENO, buf, 80)) > 0) {
            buf[--b] = 0;
            if ((strstr(buf, "addPeriod") == NULL) && (strstr(buf, "addAssignment") == NULL) && (strstr(buf, "addRevision") && (strstr(buf, "addActivity") && (strstr(buf, "addProject") == NULL) && (strstr(buf, "addBatch") == NULL) && (strstr(buf, "runS3") == NULL) && (strstr(buf, "exitS3") == NULL)))) {
                printf("Incorrect request\n");
                continue;
            }// allows the user to exit the program by typing "exitS3"
            if (strstr(buf, "exitS3") != NULL){
                if (strcmp(buf, "exitS3") == 0){
                    printf("-> Bye!\n");
                    exit(1);
                }
                else {
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
            }
            buf_count = 0;
            for ( j = 0; j < strlen(buf); j++){
                if (buf[j] == ' '){
                    buf_count++;
                }
            }//count the number of words in the inputs to check if the input is correct
            strcpy(before_def,buf);    // save the buf into a new variable to keep the buf content unchanged
            strcpy(p, strtok(before_def, " "));//get the first word of the input
            if(strcmp(p,"addPeriod")==0) {
                if (buf_count != 4){
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
                else{
                    fprintf(outfilep, "%s ", p);
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, ":"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, ":"));
                    fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                }//print the input to the text file
            } else if (strcmp(p,"addAssignment")==0) {
                if (buf_count != 3){
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
                else{
                    fprintf(outfilep, "%s ", p);
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                }//print the input to the text file
            } else if (strcmp(p,"addProject")==0) {
                if (buf_count != 3){
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
                else{
                    fprintf(outfilep, "%s ", p);
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                }//print the input to the text file
            } else if (strcmp(p,"addRevision")==0) {
                if (buf_count != 4){
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
                else{
					fprintf(outfilep, "%s ", p);
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, ":"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                }//print the input to the text file
            } else if (strcmp(p,"addActivity")==0) {
                if (buf_count != 4){
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
                else{
                    fprintf(outfilep, "%s ", p);
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s", strtok(NULL, "-"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s", strtok(NULL, ":"));
                    fprintf(outfilep, "%s ", strtok(NULL, " "));
                    fprintf(outfilep, "%s\n", strtok(NULL, "\n"));  
                }//print the input to the text file              
            } else if (strcmp(p,"addBatch")==0) {
                if (buf_count != 1){	
                    printf("Incorrect request\n");
                    continue;
                }//incorrect input handler
                else{
                    strcpy(filename, strtok(NULL, " "));    // get the .dat filename
                    infile = fopen(filename,"r");    // open the .dat file for reading the requests
                    // print out an error message if there is problem with the .dat file
                    if (infile == NULL){
                        printf("Error in opening input file\n");
                        continue;
                    }//error handle
                    infile = fopen(filename,"r");//open the input file
                    if (infile == NULL) {
                        printf("File open failure\n");
                        exit(1);
                    }//error handle
                    while (fgets(bat,100,infile) != NULL) {
                        strcpy(p, strtok(bat, " "));
                        if(strcmp(p,"addPeriod")==0) {
                            fprintf(outfilep, "%s ", p);
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, ":"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, ":"));
                            fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                        } else if (strcmp(p,"addAssignment")==0) {
                            fprintf(outfilep, "%s ", p);
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                        } else if (strcmp(p,"addProject")==0) {
                            fprintf(outfilep, "%s ", p);
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                        } else if (strcmp(p,"addRevision")==0) {
                            fprintf(outfilep, "%s ", p);
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, ":"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s\n", strtok(NULL, "\n"));
                        } else if (strcmp(p,"addActivity")==0) {
                            fprintf(outfilep, "%s ", p);
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s", strtok(NULL, "-"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s", strtok(NULL, ":"));
                            fprintf(outfilep, "%s ", strtok(NULL, " "));
                            fprintf(outfilep, "%s\n", strtok(NULL, "\n"));//print the input to the text file
                        }
                    }
                }
                fclose(infile);//close the input file
            } else if (strcmp(p,"runS3")==0) {
                fclose(outfilep);//close output file pointer
                filetxt = fopen("file.txt","r"); //input file.txt(the file stores all commands)and compile
				strtok(NULL, " ");
                strcpy(fname, strtok(NULL, " "));//get the file name from the input
                logfile=fopen("S3_FCFS.log","w");//open the log file
				if (logfile == NULL) {
					printf("Error in making log file\n");
					exit(1);
				}//error handler
                fprintf(logfile, "***Log File -FCFS***\n");
                fprintf(logfile, "ID Event Accepted/Rejected\n");
                fprintf(logfile, "**************************\n");
                while (fgets(bat,100,filetxt) != NULL) {//read lines from the text file
					count++;//count the number of requests
                    write(fd[1],bat,strlen(bat));//send the line to child
                    back = read(c[0],s,80);//get the status from child
					bat[strlen(bat)-1]=0;
                    s[back]=0;
                    fprintf(logfile, "%d %s %s\n",count,bat,s);//print log
                }
				strcpy(bat,"summary ");
				strcat(bat,fname);
				write(fd[1],bat,strlen(bat));//send the report file name to child
                fclose(logfile);
            } else if (strcmp(p,"exitS3")==0) {
                break;
            } else {
                printf("error\n");
            }//error handler
            printf("Please enter\n");
        }
        close(fd[1]);
        close(c[0]);//close pipes
        wait(NULL);//wait for child
    }
    exit(0);
}