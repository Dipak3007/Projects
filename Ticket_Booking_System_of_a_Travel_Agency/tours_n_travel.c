#include<stdio.h>
#include<mysql/mysql.h>
#include<stdlib.h>
#include<string.h>

void error_check(MYSQL *ptr)										//Function to print errors
{
	fprintf(stderr, "%s\n", mysql_error(ptr));
	mysql_close(ptr);
	exit(1);
}

char* tostr(int x)											//Function to Convert integer into string
{
	char *str= (char *) malloc (15);
	sprintf(str,"%d",x);
	return str;
}

void showad();												//Prototypes of all functions which'll be used later
void bookbus(MYSQL *);
int showroute(MYSQL *);
void showseat(MYSQL *,int);
void printticket(MYSQL *);
void addroute(MYSQL *);

int main()
{
	MYSQL *ptr =mysql_init(NULL);									//Initialising a pointer of MYSQL datatype
	if (ptr == NULL)
	error_check(ptr);
	
	system("clear");
	char uname[30],pass[30],database[30];
	printf("Enter Username : ");									//Username of mysql to be provided
	fflush(stdin);
	scanf("%[^\n]%*c",uname);
	printf("Enter Password : ");									//Password of mysql to be provided
	fflush(stdin);
	scanf("%[^\n]%*c",pass);
	printf("Enter Database Name : ");								//Name of Database used to be provided
	fflush(stdin);
	scanf("%[^\n]%*c",database);
	if (mysql_real_connect(ptr,"localhost",uname,pass,database,0,NULL,0) == NULL)		//Connection with MySQL is made here
	error_check(ptr);
	else
	{
		system("clear");
		printf("\nConnection successfull...!!!\n");
	}
	
	int choice;											//Menu of the Software
	do
	{
	printf("\n1. Book Ticket\n2. Print Ticket\n3. Show Routes\n4. Add Route\n5. Exit");
	printf("\n\nEnter Your Option : ");
	scanf("%d",&choice);
	switch(choice)
	{
		case 1: bookbus(ptr);
			break;
		case 2: printticket(ptr);
			break;
		case 3: showroute(ptr);
			break;
		case 4: addroute(ptr);
			break;
		case 5: exit(0);
		default : system("clear");
			  printf("\nWrong Option...!!!\n");
	}
	}while(choice!=5);
	return 0;
}

void showad()												//Function to show Advertisement of the travel Company
{
	printf("\n\n\t\t\t\t\t\t\t\t\tXYZ Tours & Travel");
	printf("\n\t\t\t\t\t\t\t  'Enjoy your Trip, and leave everything for us'");
	printf("\n\n\t\t\t\t\t\t\t\tA Gaya Marg, Mahim, Mumbai, 400016");
	printf("\n\t\t\t\t\t\t\t\t\tcontact: 9876543210");
	printf("\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

int showroute(MYSQL *ptr)										//Function to print all routes
{
	if (mysql_query(ptr, "SELECT * FROM route;"))
	error_check(ptr); 
	MYSQL_RES *result = mysql_store_result(ptr);
	if (result == NULL)
	error_check(ptr);
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	
	system("clear");
	showad();
	printf("\n\n\t\t\tSl. no.\t\tRoute_Name\t\tFrom\t\tTo\t\tPrice Per Seat\t\tTime of Departure");
	printf("\n\t\t\t--------\t----------------\t--------\t---------\t---------------\t\t--------------------\n\t\t\t");
	int x=0;
	while ((row = mysql_fetch_row(result)))
	{
		for(int i = 0; i < num_fields; i++)
		printf("%s\t\t", row[i]);
		printf("\n\t\t\t");
		x++;
	}
	printf("\n\n\n");
	return x;
}

void showseat(MYSQL *ptr,int r)									//Function to print seats available
{
	char *sqls= (char *) malloc (200);
	strcpy(sqls, "select id,seat_number from seat where route_id=");
	strcat(sqls, tostr(r));
	strcat(sqls, " and available=1;");
	if (mysql_query(ptr, sqls))
	error_check(ptr); 
	MYSQL_RES *result = mysql_store_result(ptr);
	if (result == NULL)
	error_check(ptr);
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	
	printf("\n\nID\tSeats Available");
	printf("\n---\t---------------\n");
	while ((row = mysql_fetch_row(result)))
	{
		for(int i = 0; i < num_fields; i++)
		printf("%s\t", row[i]);
		printf("\n");
	}
}

void bookbus(MYSQL *ptr)										//Function to Book the tickets
{
	char name[30],phone[11];
	printf("Enter Customer Name : ");								//getting customer details
	getchar();
	gets(name);
	printf("Enter Contact Number : ");
	gets(phone);
	int x=showroute(ptr);										//showing routes available
	int choice,n,i;
	op :
	printf("\n\nEnter Route Number you want to travel : ");
	scanf("%d",&choice);
	if(choice>x)
	{
		printf("\nWrong Choice...!!!\nPlease, try again...");
		goto op;
	}
	showseat(ptr,choice);										//showing seats available in the given route
	printf("\nEnter Number of Seats : ");
	scanf("%d",&n);
	int seat[n];
	printf("\nEnter Seat_IDs you want to book : \n");
	for(i=0;i<n;i++)
	scanf("%d",&seat[i]);
	
	char *sqls= (char *) malloc (200);
	strcpy(sqls, "INSERT INTO purchase(`customer_name`,`phone_number`,`number_of_seats`,`total_price`,`route_id`,time_of_booking) VALUES('");
	strcat(sqls,name);
	strcat(sqls,"','");
	strcat(sqls,phone);
	strcat(sqls,"',");
	strcat(sqls, tostr(n));
	strcat(sqls,",");
	strcat(sqls, tostr(n));
	strcat(sqls,"*(select fair_per_person from route where id=");
	strcat(sqls, tostr(choice));
	strcat(sqls,"),");
	strcat(sqls, tostr(choice));
	strcat(sqls,",current_timestamp());");
	
	/*string sqls is containing the query used for insertion
	  ie. INSERT INTO purchase(`customer_name`,`phone_number`,`number_of_seats`,`total_price`,`route_id`,time_of_booking) 
	  VALUES('name','phone',n,n*(select fair_per_person from route where id=choice),choice,urrent_timestamp()); */
	
	if (mysql_query(ptr, sqls)) 									//Insertion in purchase table
	error_check(ptr);
	
	char *sqls1= (char *) malloc (200);
	strcpy(sqls1, "update seat set available=0, pid=(select id from purchase where phone_number='");
	strcat(sqls1,phone);
	strcat(sqls1,"') where id in(");
	for(i=0;i<n-1;i++)
	{
		strcat(sqls1,tostr(seat[i]));
		strcat(sqls1,",");
	}
	strcat(sqls1,tostr(seat[i]));
	strcat(sqls1,");");
	
	/*string sqls1 is containing the query used for updation of availability of seats
	  ie. update seat set available=0, pid=(select id from purchase where phone_number='phone') where id in(seat[0],seat[1],...,seat[n-1]); */

	if (mysql_query(ptr, sqls1))									//updation in seat table
	error_check(ptr);
	
	char *total= (char *) malloc (20);
	system("clear");
	showad();
	char *sqls2= (char *) malloc (200);
	strcpy(sqls2, "select purchase.id,customer_name,phone_number,route_name,route.from,route.to,total_price,time_of_departure,time_of_booking from purchase,route where phone_number='");
	strcat(sqls2,phone);
	strcat(sqls2,"' and route_id=route.id;");
	
	/*string sqls2 is containing the query used for retrieval of data for ticket generation
	  ie. select purchase.id,customer_name,phone_number,route_name,route.from,route.to,total_price,time_of_departure,time_of_booking from purchase,route
	  where phone_number='phone' and route_id=route.id; */

	if (mysql_query(ptr,sqls2))
	error_check(ptr); 
	MYSQL_RES *result = mysql_store_result(ptr);
	if (result == NULL)
	error_check(ptr);
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		printf("\n\n\n\t\t\t\t\t\tPurchase ID\t  : %s",row[0]);
		printf("\n\t\t\t\t\t\tTime of Purchase  : %s",row[8]);
		printf("\n\t\t\t\t\t\tCustomer Name\t  : %s",row[1]);
		printf("\n\t\t\t\t\t\tContact Number\t  : %s",row[2]);
		printf("\n\t\t\t\t\t\tRoute Name\t  : %s",row[3]);
		printf("\n\n\t\t\t\t\t\tFrom\t\t  : %s\t\t\tTo\t: %s",row[4],row[5]);
		printf("\n\t\t\t\t\t\tTime of Departure : %s",row[7]);
		strcpy(total,row[6]);
	}
	printf("\n\n\t\t\t\t\t   ------------------------------------------------------------------------------");
	printf("\n\t\t\t\t\t   ------------------------------------------------------------------------------");
	printf("\n\n\t\t\t\t\t\t\t\tSeats Purchased\t\tCost");
	printf("\n\t\t\t\t\t\t\t\t_________________\t________\n\n");
	
	char *sqls3= (char *) malloc (200);
	strcpy(sqls3, "select seat_number,fair_per_person from seat,route,purchase where purchase.id=pid and route.id=purchase.route_id and phone_number='");
	strcat(sqls3,phone);
	strcat(sqls3,"';");
	
	/*string sqls3 is containing the query used for printing seats which are booked by this customer
	  ie. select seat_number,fair_per_person from seat,route,purchase where purchase.id=pid and route.id=purchase.route_id and phone_number='phone'; */
	
	if (mysql_query(ptr,sqls3))
	error_check(ptr); 
	result = mysql_store_result(ptr);
	if (result == NULL)
	error_check(ptr);
	num_fields = mysql_num_fields(result);
	while ((row = mysql_fetch_row(result)))
	{
		printf("\t\t\t\t\t");
		for(int i = 0; i < num_fields; i++)
		printf("\t\t\t%s",row[i]);
		printf(" /-\n");
	}
	printf("\n\n\n\t\t\t\t\t   ------------------------------------------------------------------------------");
	printf("\n\t\t\t\t\t\t\t\t\t\tTotal : %s",total);
	printf("\n\t\t\t\t\t   ------------------------------------------------------------------------------\n\n");
	printf("\n\n\n\t\t\t\t\t\t\t\t\t   Thank You!!!");
	printf("\n\t\t\t\t\t\t\t\t\tHave A Safe Journey\n\n\n\n\n\n");
}


void printticket(MYSQL *ptr)										//Function to Print the tickets
{
	int pid;
	printf("\nEnter Purchase ID : ");
	scanf("%d",&pid);
	char *total= (char *) malloc (20);
	system("clear");
	showad();
	char *sqls2= (char *) malloc (200);
	strcpy(sqls2, "select purchase.id,customer_name,phone_number,route_name,route.from,route.to,total_price,time_of_departure,time_of_booking from purchase,route where purchase.id=");
	strcat(sqls2,tostr(pid));
	strcat(sqls2," and route_id=route.id;");
	
	/*string sqls2 is containing the query used for retrieval of data for ticket Printing
	  ie. select purchase.id,customer_name,phone_number,route_name,route.from,route.to,total_price,time_of_departure,time_of_booking from purchase,route
	  where purchase.id=pid and route_id=route.id; */
	
	if (mysql_query(ptr,sqls2))
	error_check(ptr); 
	MYSQL_RES *result = mysql_store_result(ptr);
	if (result == NULL)
	error_check(ptr);
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		printf("\n\n\n\t\t\t\t\t\tPurchase ID\t  : %s",row[0]);
		printf("\n\t\t\t\t\t\tTime of Purchase  : %s",row[8]);
		printf("\n\t\t\t\t\t\tCustomer Name\t  : %s",row[1]);
		printf("\n\t\t\t\t\t\tContact Number\t  : %s",row[2]);
		printf("\n\t\t\t\t\t\tRoute Name\t  : %s",row[3]);
		printf("\n\n\t\t\t\t\t\tFrom\t\t  : %s\t\t\tTo\t: %s",row[4],row[5]);
		printf("\n\t\t\t\t\t\tTime of Departure : %s",row[7]);
		strcpy(total,row[6]);
	}
	printf("\n\n\t\t\t\t\t   ------------------------------------------------------------------------------");
	printf("\n\t\t\t\t\t   ------------------------------------------------------------------------------");
	printf("\n\n\t\t\t\t\t\t\t\tSeats Purchased\t\tCost");
	printf("\n\t\t\t\t\t\t\t\t_________________\t________\n\n");
	
	char *sqls3= (char *) malloc (200);
	strcpy(sqls3, "select seat_number,fair_per_person from seat,route,purchase where purchase.id=pid and route.id=purchase.route_id and purchase.id=");
	strcat(sqls3,tostr(pid));
	strcat(sqls3,";");
	
	/*string sqls3 is containing the query used for printing seats which are booked by this customer
	  ie. select seat_number,fair_per_person from seat,route,purchase where purchase.id=pid and route.id=purchase.route_id and purchase.id=pid; */
	
	if (mysql_query(ptr,sqls3))
	error_check(ptr); 
	result = mysql_store_result(ptr);
	if (result == NULL)
	error_check(ptr);
	num_fields = mysql_num_fields(result);
	while ((row = mysql_fetch_row(result)))
	{
		printf("\t\t\t\t\t");
		for(int i = 0; i < num_fields; i++)
		printf("\t\t\t%s",row[i]);
		printf(" /-\n");
	}
	printf("\n\n\n\t\t\t\t\t   ------------------------------------------------------------------------------");
	printf("\n\t\t\t\t\t\t\t\t\t\tTotal : %s",total);
	printf("\n\t\t\t\t\t   ------------------------------------------------------------------------------\n\n");
	printf("\n\n\n\t\t\t\t\t\t\t\t\t   Thank You!!!");
	printf("\n\t\t\t\t\t\t\t\t\tHave A Safe Journey\n\n\n\n\n\n");
}

void addroute(MYSQL *ptr)										//Function to Add routes
{
	char name[30],from[30],to[30],seat[10],time[20];
	int fare,n,i;
	printf("Enter Route Name : ");								//getting route details
	getchar();
	gets(name);
	printf("Enter Source Point : ");
	gets(from);
	printf("Enter Destination : ");
	gets(to);
	printf("Enter Fare Per Person : ");
	scanf("%d",&fare);
	printf("Enter Time of Departure (YYYY-MM-DD HH:MM:SS) : ");
	getchar();
	gets(time);
	
	char *sqls= (char *) malloc (200);
	strcpy(sqls, "INSERT INTO route(`route_name`,`from`,`to`,`fair_per_person`,time_of_departure) VALUES('");
	strcat(sqls,name);
	strcat(sqls,"','");
	strcat(sqls,from);
	strcat(sqls,"','");
	strcat(sqls, to);
	strcat(sqls,"',");
	strcat(sqls, tostr(fare));
	strcat(sqls,",'");
	strcat(sqls, time);
	strcat(sqls,"');");
	
	/*string sqls is containing the query used for insertion in table route
	  ie. INSERT INTO route(`route_name`,`from`,`to`,`fair_per_person`,time_of_departure)
	  VALUES('name','from','to',fare,'time'); */
	
	if (mysql_query(ptr, sqls))									//insertion in route table
	error_check(ptr);
	else
	{
		system("clear");
		printf("\nRoute Added Successfully");
	}
	
	printf("\nEnter Number of Seats : ");
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		printf("\nEnter Seat Name : ");
		getchar();
		gets(seat);
		char *sqls= (char *) malloc (200);							//insertion in seat table
		strcpy(sqls, "INSERT INTO seat(seat_number,route_id) VALUES('");
		strcat(sqls,seat);
		strcat(sqls,"',(select id from route where route_name='");
		strcat(sqls,name);
		strcat(sqls,"'));");
		
		/*string sqls is containing the query used for insertion in table seat
		  ie. INSERT INTO seat(seat_number,route_id) VALUES('seat',,(select id from route where route_name='name')); */
		
		if (mysql_query(ptr, sqls))
		error_check(ptr);
		else
		printf("\nRoute Added Successfully");
	}
}
