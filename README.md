# Study Scheduling System

This is a study scheduling system to help students to prepare a timetable to arrange those tasks that they can achieve the best result in spending the valuable time.

*One time slot is assumed as one hour.

*The program is based on Linux system and C language.

(This is a group project. Please look at [Contribution of Work](#contribution-of-work))

- [Introduction](#introduction)
- [Concept](#concept)
- [Software Structure](#software-structure)
- [Examples](#examples)
- [Algorithm](#algorithm)
- [Compilation and Execution](#compilation-and-execution)
- [Flowchart](#flowchart)
- [Contribution of Work](#contribution-of-work)

## Introduction

In this project, a programme is conducted in order to schedule activities. Near the end of each semester, students are busy with many activities and assignments. This programme aims to help those students to manage their time in an effective way. Users need to input the details of the activity, such as subject code, due date, and duration, then the programme will provide an algorithm for students to schedule their time. A log file and a summary report will be generated after the runS3 function. The acceptance or rejection of a request will be recorded in the log file, the analysis report and timetable will be recorded in the summary report.

## Concept

In this programme, the programme is in a loop that user can input any requests until they type “exitS3” to exit the loop. The programme will determine the input, there are several functions: addPeriod, addAssignment, addProject, addRevision, addActivity, addBatch, and runS3. The input apart from these functions will not be considered as a request. Also, if the format is different from stated below, the input will not be considered as a request too. All requests will be recorded in a text file.

A parent process and a child process are created in this programme. In the runS3 function, the parent process will read the file, the file will read line by line, the format of the request will be stored in the array. Then, the result will send to the child process through a pipe. 

An algorithm is established in the child process, in this programme, only one algorithm is used - first come first served. The child process will determine whether to accept or reject the request. Also, some analysis will be done by the child process and it prints the result into the summary report. Also, a timetable will be printed in the summary report.

After the determination of requests, the child process will send the result to the parent through a pipe. Parent process will receive the result and print the result to the log file. 


## Software Structure

There are 4 parts in the programme:

1.	Input Module

	There are six types of data inputs for users to input information to the scheduler, they are addPeriod, addAssignment, addProject, addRevision, addActivity and addBatch. The first 5 inputs allow users to input the data line by line in the command line, and the last one allows users to input data by reading in batch files.

	<table>
	  <tr>
		<td colspan="2">addPeriod</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To specify the starting and ending dates, and the time slots for scheduling events. This should be the first command for the application.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>addPeriod [start date] [end date] [start time] [end time]
			<br/>Date format: yyyy-mm-dd
			<br/>Time format: hh:00
		</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>addPeriod 2019-04-08 2019-04-21 19:00 23:00
			<br/>We did not provide the study scheduling out of this period.
		</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">addAssignment</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To add a subject assignment to the scheduler. Assignment can be split into multiple slot.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>addAssignment [subject code assignment no.] [due date] [duration]
			<br/>Date format: yyyy-mm-dd
			<br/>Duration: number of hours
		</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>addAssignment COMP2342A1 2019-04-18 12</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">addProject</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To add a subject project to the scheduler. Similar to assignment, project can also be split into multiple slot.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>addProject [subject code project no.] [due date] [duration]</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>addProject COMP2422P1 2019-04-20 26</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">addRevision</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To add a revision section to the scheduler. Unlike the previous two, this task should be done in one off.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>addRevision [subject code] [date and time] [duration]
			<br/>Date and time format: yyyy-mm-dd hh:00
		</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>addRevision COMP2000 2019-04-14 19:00 2</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">addActivity</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To add an activity to the scheduler.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>addActivity [name of the event] [date and time] [duration]</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>addActivity Meeting 2019-04-18 20:00 2</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">addBatch</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To allow users to prepare a text file containing lines of event and import to the scheduler.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>addBatch [filename]</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>addBatch S3_tasks_01.dat</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">runS3</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To run a scheduling process.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>runS3 [algorithm] [filename]
			<br/>The [filename] is a file that will contain the analysis report and the time table.
			<br/>Also, a log file including the acceptance or rejection of a request will be exist.
		</td>
	  </tr>
	  <tr>
		<td>Example</td>
		<td>runS3 FCFS S3_report_fcfs_01.dat
			<br/>FCFS, first come first served, can be used.</td>
	  </tr>
	</table>
	<table>
	  <tr>
		<td colspan="2">exitS3</td>
	  </tr>
	  <tr>
		<td>Usage</td>
		<td>To exit the programme.</td>
	  </tr>
	  <tr>
		<td>Format</td>
		<td>exitS3</td>
	  </tr>
	</table>

2.	Scheduling Module

	The scheduling algorithm of this scheduler implemented is First-Come-First-Serve Scheduling. This algorithm schedules events according to the event date and time. When there are two events with the same date and time, by FCFS, the later input would be rejected.

3.	Output Module

	After scheduling, the events are accepted or rejected based on the FCFS algorithm. This programme generates a log file and a summary report.

4.	Analyzer Module

	The log file generated includes the list of acceptance and rejection of every request, and the summary report includes the numbers of requests, acceptance and rejection, number of timeslots used with a percentage, and the timetable generated.

## Examples

- [Inputs](https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/s3_data.dat)

	<img src="https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/imgs/inputs.png" width="700">
	
- [Summary Report](https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/S3_report_FCFS.dat)

	<img src="https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/imgs/report.png" width="700">

- [Log file](https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/S3_FCFS.log)

	<img src="https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/imgs/log.png" width="700">

## Algorithm

FCFS, first come first served is used in this programme.  This algorithm is based on the input order of the requests. The first input request will be accepted. There are total of 56 timeslots, due to the limitation of time slots, if the remaining time slots are not enough for a request, the request will be rejected. We indicated slot = 56 before coding, this is an important parameter to determine the approval of the request.

## Compilation and Execution

The compiler GNU Compiler Collection (gcc) is used in this programme, most of the operating systems use gcc as their standard compiler, it supports many programming languages such as C, C++, and Java. In this programme, programming language C is used.

## Flowchart

<img src="https://github.com/rf1112/Portfolio/blob/main/Study_Scheduling_System/imgs/flowchart.png" width="750">

## Contribution of Work

- User inputs

	I worked on reading and splitting user inputs and storing the inputs to the text file.
	
- Summary Report
	
	I worked on generating a summary report which includes the numbers of total, accepted and rejected requests, number of timeslots used with percentage, and the timetable.
	
- Testing, Debugging and Commenting

	I put the codes from other members together and worked on testing and debugging.
	Also, I commented the code to make it clearer for other people to understand what it actually does.
