#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

MYSQL* connection = NULL;
MYSQL conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;

void type1(void) {
	int type_option, quit_query = 0;
	int truck_num;
	printf("------------- TYPE I -------------\n\n");
	while (1) {
		printf("Input the number of truck : ");
		scanf("%d", &truck_num);
		if (truck_num == 0)
			return;
		else if (truck_num == 1721)
			break;
		else
			printf("Truck %04d is not destropyed\n\n", truck_num);
	}
	while (1) {
		printf("------- Subtypes in TYPE I -------\n\n");
		printf("\t1. TYPE I-1.\n");
		printf("\t2. TYPE I-2.\n");
		printf("\t3. TYPE I-3.\n");
		printf("----------------------------------\n\n");

		while (1) {
			printf("Which type of query? : ");
			scanf("%d", &type_option);


			if (type_option == 1) {
				printf("---- TYPE I-1 ----\n\n");
				printf("* Truck 1721 is destroyed in a crash. *\n");
				printf("** Find all customer who had a package on the truck at the time of the crash. **\n\n");

				const char* query = "select customer_name from customer where customer_ID in(select customer_ID from shipment where shipment_ID in(select tracking.shipment_ID from tracking, ontruck where tracking.tracking_ID = ontruck.tracking_ID and tracking.shipment_ID = ontruck.shipment_ID and truck_ID = '1721' and tracking.delivery_state = 'failure' and tracking.date = '2020-06-07')) ";
				int state = 0, cnt = 0;
				printf("Customer Name : ");
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						if (!cnt)
							printf("%s", sql_row[0]);
						else
							printf(", %s", sql_row[0]);
						cnt++;
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}
			}
			else if (type_option == 2) {
				printf("---- TYPE I-2 ----\n\n");
				printf("*Assume truck 1721 is destroyed in a crash.*\n");
				printf("** Find all recipients who had a package on the truck at the time of the crash **\n\n");

				const char* query = "select recipient from shipment where shipment_ID in(select tracking.shipment_ID from tracking, ontruck where tracking.tracking_ID = ontruck.tracking_ID and tracking.shipment_ID = ontruck.shipment_ID and truck_ID = '1721' and tracking.delivery_state = 'failure' and tracking.date = '2020-06-07')";
				int state = 0;
				printf("Recipient Name : ");
				state = mysql_query(connection, query);
				int cnt = 0;
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						if (!cnt)
							printf("%s", sql_row[0]);
						else
							printf(", %s", sql_row[0]);
						cnt++;
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}
			}
			else if (type_option == 3) {
				printf("---- TYPE I-3 ----\n\n");
				printf("*Assume truck 1721 is destroyed in a crash.*\n");
				printf("** Find the last successful delivery by that truck prior to the crash. **\n\n");

				const char* query = "select shipment_ID,customer_ID,arrival_date,recipient from shipment where shipment_ID in (select tracking.shipment_ID from tracking, ontruck where tracking.tracking_ID = ontruck.tracking_ID and tracking.shipment_ID = ontruck.shipment_ID and truck_ID = '1721' and delivery_state = 'completed')";
				int state = 0;

				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					printf("============================================================\n");
					printf("shipmet_ID\tcustomer_ID\tarrival_date\trecipient\n");
					printf("============================================================\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t%s\t\t%s\t%s\n\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					mysql_free_result(sql_result);
				}
			}
			else if (type_option == 0) { // 0 입력 시 종료
				return;
			}
			else {
				printf("Please enter 0 to 3!\n");
			}

		}
	}
}
void type2(void) {
	int year;
	printf("---- TYPE II ----\n\n");
	printf("** Find the customer who has shipped the most packages in the past certain year **\n\n");

	while (1) {
		printf(" Which Year? : ");
		scanf("%d", &year);
		if (year == 0) //0 입력 시 종료
			return;
		printf(" Customer Name : ");

		char query1[360];
		sprintf(query1, "with cnt_shipped(customer_ID, cnt) as (select customer_ID, count(*) from shipment where arrival_date > '%4d-01-01'	and arrival_date < '%4d-12-31' group by customer_ID) select customer_name from customer, cnt_shipped where customer.customer_ID = cnt_shipped.customer_ID and cnt_shipped.cnt = (select max(cnt) from cnt_shipped)", year, year);
		int state = 0;
		int cnt = 0;
		state = mysql_query(connection, query1);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				if (cnt == 0)
					printf("%s", sql_row[0]);
				else
					printf(", %s", sql_row[0]);
				cnt++;
			}
			mysql_free_result(sql_result);
		}
		if (cnt == 0)
			printf("There is no result!\n\n");
		else
			printf("\n\n");
	}
}
void type3(void) {
	int year;
	printf("---- TYPE III ----\n\n");
	printf("** Find the customer who has spent the most money on shipping in the past certain year **\n");
	while (1) {
		printf(" Which Year? : ");
		scanf("%d", &year);
		if (!year) //year이 0이면 종료
			break;
		printf(" Customer Name : ");

		const char* query = "with cnt_shipped(customer_ID, cnt) as (select customer_ID, count(*) from shipment where arrival_date > '2015-01-01'	and arrival_date < '2015-12-31' group by customer_ID) select customer.customer_ID, customer_name from customer, cnt_shipped where customer.customer_ID = cnt_shipped.customer_ID and cnt_shipped.cnt = (select max(cnt) from cnt_shipped)";
		char query1[370];
		sprintf(query1, "with year_cost(customer_ID, cost) as (select shipment.customer_ID, sum(amount) from shipment, bill where year = '%4d' and shipment.shipment_ID = bill.shipment_ID group by shipment.customer_ID) select customer_name from customer, year_cost where customer.customer_ID = year_cost.customer_ID and year_cost.cost = (select max(cost) from year_cost) ", year);
		int state = 0, cnt = 0;
		state = mysql_query(connection, query1);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				if (!cnt)
					printf("%s", sql_row[0]);
				else
					printf(", %s", sql_row[0]);
				cnt++;
			}
			if (!cnt)
				printf("No result!\n");
			else
				printf("\n\n");
			mysql_free_result(sql_result);
		}
	}
}
void type4(void) {
	printf("---- TYPE IV ----\n\n");
	printf("**Find those packages that were not delivered within the promised time. **\n\n");

	printf(" Package_ID : ");

	const char* query = "select package.package_ID from package, shipment where package.package_ID = shipment.package_ID and package.predict_arrival_date < shipment.arrival_date";

	int state = 0, cnt = 0;
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			if (!cnt)
				printf("%s", sql_row[0]);
			else
				printf(", %s", sql_row[0]);

			cnt++;
		}
		printf("\n\n");
		mysql_free_result(sql_result);
	}

}
void type5(void) {
	char name[26];
	int year, month;
	printf("---- TYPE V ----\n\n");
	printf("** Generate the bill for each customer for the past certain month. **\n\n");

	while (1) {
		char filename[50];
		printf("\n Enter the Customer name : ");
		scanf("%s", name);

		if (strcmp(name, "0") == 0) //0입력 시 종료
			break;
		printf(" Which year(YYYY)? : ");
		scanf("%d", &year);
		if (year == 0)
			break;
		printf(" Which month(MM)? : ");
		scanf("%d", &month);
		if (month == 0)
			break;

		sprintf(filename, "bill_%4d%02d_%s.txt", year, month, name);

		printf("Generating Bil..\n");
		FILE* fp = fopen(filename, "w");
		char query1[250];
		sprintf(query1, "select customer_name, address, sum(amount) from bill, customer where year = '%4d' and month = '%02d' and customer_name = '%s' and customer.customer_ID = bill.customer_ID group by customer.customer_ID", year, month, name);
		int state = 0, cnt = 0;
		fprintf(fp, "Customer |\tAddress |\tAmount\n");
		state = mysql_query(connection, query1);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				fprintf(fp, "%s |\t%s |\t\\ %s \n", sql_row[0], sql_row[1], sql_row[2]);
				cnt++;
			}

			mysql_free_result(sql_result);
		}
		if (cnt == 0) {
			printf("There is no result! Please enter the other information\n");
			fprintf(fp, "There is no result! \n");
			fclose(fp);
		}
		else {
			fprintf(fp, "\n================================\n");
			fprintf(fp, "  Bill list by type of service  \n");
			fprintf(fp, "================================\n");
			fprintf(fp, "Package  |\tAmount |\tService Type\n");
			fprintf(fp, "----------------------------------------\n");
			char query2[300];
			float weight;
			sprintf(query2, "select package.package_ID, weight, amount from bill, customer, package, shipment where customer_name = '%s' and year = '%4d' and month = '%02d' and customer.customer_ID = bill.customer_ID and bill.shipment_ID = shipment.shipment_ID and shipment.package_ID = package.package_ID", name, year, month);
			state = mysql_query(connection, query2);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					fprintf(fp, "%s |\t\\ %s |\t ", sql_row[0], sql_row[2]);
					sscanf(sql_row[1], "%f", &weight);
					if (weight < 1.2)
						fprintf(fp, "Flat Envelope\n");
					else if (weight < 5.8)
						fprintf(fp, "Small Box\n");
					else
						fprintf(fp, "Large Box\n");
				}

				mysql_free_result(sql_result);
			}

			fprintf(fp, "\n================================\n");
			fprintf(fp, "    Itemize billing list  \n");
			fprintf(fp, "================================\n");
			char query3[350];
			sprintf(query3, "select package.package_ID, weight, amount, pay_type, montly_type, predict_arrival_date, arrival_date from bill, customer, package, shipment where customer_name = '%s' and year = '%4d' and month = '%02d' and customer.customer_ID = bill.customer_ID and bill.shipment_ID = shipment.shipment_ID and shipment.package_ID = package.package_ID ", name, year, month);
			state = mysql_query(connection, query3);
			fprintf(fp, "Package  |\tAmount |\tPayment Type |\tMontly type |\tWeight |\tTimeline\n");
			fprintf(fp, "------------------------------------------------------------------------------------------------\n");

			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					fprintf(fp, "%s |\t\\ %s |\t%s |\t%s |\t ", sql_row[0], sql_row[2], sql_row[3], sql_row[4]);
					sscanf(sql_row[1], "%f", &weight);
					fprintf(fp, "%.2f |\t", weight);

					if (strcmp(sql_row[5], sql_row[6]))
						fprintf(fp, "delivered on time\n");
					else
						fprintf(fp, "delayed\n");
				}
				mysql_free_result(sql_result);
			}

			printf("Generation Completed\n\n");
			fprintf(fp, "\n");
			fclose(fp);
		}
	}
}

int main(void) {
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		//create table and insert date
		FILE* fp = fopen("input_1.txt", "r");
		char CRU[500];
		while (1) {
			if (feof(fp)) break;
			fgets(CRU, 500, fp);
			mysql_query(connection, CRU);
		}
		if (CRU != NULL)
			mysql_query(connection, CRU);
		fclose(fp);
		printf("Create table and Insert data Complete!\n");

		int type_option;
		int quit_flag = 0;


		while (1) {
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE I\n");
			printf("\t2. TYPE II\n");
			printf("\t3. TYPE III\n");
			printf("\t4. TYPE IV\n");
			printf("\t5. TYPE V\n");
			printf("\t0. QUIT\n");
			printf("----------------------------------\n");
			printf("Which type of query? : ");
			scanf("%d", &type_option);
			printf("\n");
			switch (type_option) {
			case 0:
				quit_flag = 1;
				break;
			case 1:
				type1();
				break;
			case 2:
				type2();
				break;
			case 3:
				type3();
				break;
			case 4:
				type4();
				break;
			case 5:
				type5();
				break;
			default:
				printf("input 0~5!\n");
				break;
			}
			if (quit_flag)
				break;
		}


		//delete and drop the table
		fp = fopen("input_2.txt", "r");
		char DEL[50];
		while (1) {
			if (feof(fp)) break;
			fgets(DEL, 500, fp);
			mysql_query(connection, DEL);
		}
		fclose(fp);
		printf("Delete data and Drop the table Complete!\n");

	}
	return 0;
}