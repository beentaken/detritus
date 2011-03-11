
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pvm3.h"

const int	MAXN = 30;     // Max neurons in any layer
const int	MAXPATS = 250; // Max training patterns
const int	LINE_LEN = 500;
const int	MSG_ID = 31337;

float **
Aloc2DAry(int m, int n) {
	// Allocates memory for 2D array
	float	**Ary2D = (float **) malloc(sizeof(float *) * m);

	if (!Ary2D) {
		perror("Aloc2DAry");
		exit(1);
	} else {

		for(int i = 0; i < m; i++){
			Ary2D[i]= (float *) malloc(sizeof(float) * n);

			if (!Ary2D[i]) {
				perror("Aloc2DAry");
				exit(1);
			}
		}
	}

	return(Ary2D);
}

void
Free2DAry(float **Ary2D, int n){
	for(int i=0; i < n; i++) {
		free(Ary2D[i]);
	}

	free(Ary2D);
}

int
ShowErrors(void) {
	int	s_tid, s_msg_id, bytes, buff_id;

	float	MinErr, AveErr, MaxErr;

	buff_id = pvm_recv(-1, MSG_ID);

	if (buff_id < 0) {
		pvm_perror((char *) "ShowErrors");
		exit(1);
	}

	pvm_bufinfo(buff_id, &bytes, &s_msg_id, &s_tid);

	pvm_upkfloat(&MinErr, 1, 1);
	pvm_upkfloat(&AveErr, 1, 1);
	pvm_upkfloat(&MaxErr, 1, 1);

	printf("%10d:\t%e\t%e\t%e\n", s_tid, MinErr, AveErr, MaxErr);

	return(s_tid);
}

void
AvoidLocalMinimums(int NumPats, int NumIPs, int NumOPs, float **IPData, float **OPData) {
	int	num_slaves;
	int	*task_ids;
	int	my_tid;
	int	i;
	int	info;

	printf("\nHow many slaves do you want to spawn? ");
	scanf("%d", &num_slaves);

	task_ids = (int *) calloc(sizeof(int), num_slaves);

	/* enroll in PVM */
	my_tid = pvm_mytid();

	info = pvm_spawn((char *)"slave", (char**) 0, PvmTaskDefault, (char *)"", num_slaves, task_ids);

	if (info != num_slaves) {
		pvm_perror((char *) "AvoidLocalMinimums");
		exit(1);
	}

	pvm_initsend(PvmDataDefault);

	pvm_pkint(&NumPats, 1, 1);
	pvm_pkint(&NumIPs, 1, 1);
	pvm_pkint(&NumOPs, 1, 1);

	for(i = 0; i < NumPats; i++){
		pvm_pkfloat(IPData[i], NumIPs, 1);
		pvm_pkfloat(OPData[i], NumOPs, 1);
	}

	info = pvm_mcast(task_ids, num_slaves, MSG_ID);

	printf("%3c %10s %7s %7s %7s\n", '#', "TID", "MIN", "AVE", "MAX");

	// Wait for data back from the slaves
	for (i = 0; i < num_slaves; i++) {
		int	s_tid, s_msg_id, bytes, buff_id;

		float	MinErr, AveErr, MaxErr;

		buff_id = pvm_recv(task_ids[i], MSG_ID);

		if (buff_id < 0) {
			pvm_perror((char *) "ShowErrors");
			exit(1);
		}

		pvm_bufinfo(buff_id, &bytes, &s_msg_id, &s_tid);

		pvm_upkfloat(&MinErr, 1, 1);
		pvm_upkfloat(&AveErr, 1, 1);
		pvm_upkfloat(&MaxErr, 1, 1);

		printf("%3d %10d %7.5f %7.5f %7.5f\n", i, s_tid, MinErr, AveErr, MaxErr);
	}

	free(task_ids);
}

void
DistributeOutputs(int NumPats, int NumIPs, int num_slaves,  float **IPData, float **OPData) {
	int	NumOPs = 1;
	int	*task_ids;
	int	my_tid;
	int	i, j;
	int	info;

	task_ids = (int *) calloc(sizeof(int), num_slaves);

	/* enroll in PVM */
	my_tid = pvm_mytid();

	info = pvm_spawn((char *)"slave", (char**) 0, PvmTaskDefault, (char *)"", num_slaves, task_ids);

	if (info != num_slaves) {
		pvm_perror((char *) "DistributeOutputs");
		exit(1);
	}

	for (i = 0; i < num_slaves; i++) {
		pvm_initsend(PvmDataDefault);

		pvm_pkint(&NumPats, 1, 1);
		pvm_pkint(&NumIPs, 1, 1);
		pvm_pkint(&NumOPs, 1, 1);

		for(j = 0; j < NumPats; j++){
			pvm_pkfloat(IPData[j], NumIPs, 1);
			pvm_pkfloat(&OPData[j][i], NumOPs, 1);
		}

		pvm_send(task_ids[i], MSG_ID);
	}

	// Wait for data back from the slaves

	printf("%3c %3s %7s %7s %7s\n", '#', "OP", "MIN", "AVE", "MAX");

	for (i = 0; i < num_slaves; i++) {
		int	s_tid, s_msg_id, bytes, buff_id;

		float	MinErr, AveErr, MaxErr;

		buff_id = pvm_recv(task_ids[i], MSG_ID);

		if (buff_id < 0) {
			pvm_perror((char *) "ShowErrors");
			exit(1);
		}

		pvm_bufinfo(buff_id, &bytes, &s_msg_id, &s_tid);

		pvm_upkfloat(&MinErr, 1, 1);
		pvm_upkfloat(&AveErr, 1, 1);
		pvm_upkfloat(&MaxErr, 1, 1);

		printf("%3d %3d %7.5f %7.5f %7.5f\n", i, i, MinErr, AveErr, MaxErr);
	}

	free(task_ids);
}

void
TrainNeuralNetwork(int NumPats, int NumIPs, int NumOPs, float **IPData, float **OPData) {
	int	menu_choice;

	printf("\nPlease select one option:\n  1 => Avoid local minumums\n  2 => Distribute outputs\nEnter option: ");

	scanf("%d", &menu_choice);

	switch (menu_choice) {
		case	1:	AvoidLocalMinimums(NumPats, NumIPs, NumOPs, IPData, OPData);
				break;

		case	2:	DistributeOutputs(NumPats, NumIPs, NumOPs, IPData, OPData);
				break;
	}
}

int
factorial(int n) {
	int	total = 1;

	while (n) {
		total *= n--;
	}

	return(total);
}
		
int
product_series(int n, int m) {
	int	total = 1;

	while (m <= n) {
		total *= m++;
	}

	return(total);
}

int
C(int n, int r) {
	int	rtval = 0;

	if (r < n / 2) {
		rtval = product_series(n, n - r + 1) / factorial(r);
	} else {
		rtval = product_series(n, r + 1) / factorial(n - r);
	}

	return(rtval);
}

char *
GenerateNextIPPayload(int NumPats, int NumIPs, int NumOPs, float **IPData, float **OPData, int num_pcomponents) {
	static int	total_permutations = 0;
	static int	*cur_permutation = NULL;
	static int	cur_pcomponents = 2;

	char	*permutation = (char *)calloc(41, sizeof(char));

	int 	i, j;

	float	**inputs = NULL;

	total_permutations++;

	if (!cur_permutation) {
		cur_permutation = (int *) calloc(cur_pcomponents, sizeof(int));

		for (i = 0; i < cur_pcomponents; i++) {
			cur_permutation[i] = i;
		}

		if (!cur_permutation) {
			perror("GenerateNextIPPayload");
			pvm_exit();
			exit(1);
		}
	}

#ifdef _DEBUG
	fprintf(stderr, "total_permutations = %d\ncur_permutation[%d] =", total_permutations, cur_pcomponents);

	for (i = 0; i < cur_pcomponents; i++) {
		fprintf(stderr, "\t%d", cur_permutation[i]);
	}

	fprintf(stderr, "\n");

	fflush(stderr);
#endif

	for (i = 0; i < cur_pcomponents - 1; i++) {
		j = strlen(permutation);
		snprintf(permutation + j, 41 - j, "%d,", cur_permutation[i] + 1);
	}

	j = strlen(permutation);
	snprintf(permutation + j, 41 - j, "%d", cur_permutation[i] + 1);

	inputs = Aloc2DAry(NumPats, cur_pcomponents);

	// Generate the Payload
	for (i = 0; i < NumPats; i++) {
		for (j = 0; j < cur_pcomponents; j++) {
			inputs[i][j] = IPData[i][cur_permutation[j]];
		}
	}

	// Sort out the next permutation
	i = cur_pcomponents - 1;

	while (i >= 0) {
		if (cur_permutation[i] != NumIPs - (cur_pcomponents - i)) {
			break;
		} else {
			i--;
		}
	}

	// We have finished with this level of pcomponents
	if (i == -1) {
		cur_pcomponents++;
		free(cur_permutation);
		cur_permutation = NULL;
	} else {
		// Shake the cur_permutation array
		cur_permutation[i]++;

		for (i++; i < cur_pcomponents; i++) {
			cur_permutation[i] = cur_permutation[i - 1] + 1;
		}
	}

	pvm_initsend(PvmDataDefault);

	pvm_pkint(&NumPats, 1, 1);
	pvm_pkint(&cur_pcomponents, 1, 1);
	pvm_pkint(&NumOPs, 1, 1);

	for(i = 0; i < NumPats; i++){
		pvm_pkfloat(inputs[i], cur_pcomponents, 1);
		pvm_pkfloat(OPData[i], NumOPs, 1);
	}

	Free2DAry(inputs, cur_pcomponents);

	return(permutation);
}

void
DiscoverPrincipalComponents(int NumPats, int NumIPs, int NumOPs, float **IPData, float **OPData) {
	const int	max_slaves = 20;

	int	total_slaves, num_slaves, num_pcomponents, completed_slaves, i, info;

	int	*task_ids;

	char	*IPs[max_slaves];

	printf("\nEnter number of principal component inputs: ");

	scanf("%d", &num_pcomponents);
	
	if (num_pcomponents < 2) {
		printf("\nNot enough principal components to look for.\n");
		return;
	}

	if (num_pcomponents > NumIPs) {
		printf("\nToo many principal components to look for.\n");
		return;
	}

	total_slaves = 0;

	for (i = 2; i <= num_pcomponents; i++) {
		total_slaves += C(NumIPs, i);
	}

#ifdef _DEBUG
	fprintf(stderr, "Total slaves = %d\n", total_slaves);
#endif

	num_slaves = total_slaves < max_slaves ? total_slaves : max_slaves;

	completed_slaves = 0;
	
	task_ids = (int *) calloc(num_slaves, sizeof(int));

	info = pvm_spawn((char *)"slave", (char**) 0, PvmTaskDefault, (char *)"", num_slaves, task_ids);

	if (info != num_slaves) {
		pvm_perror((char *) "DiscoverPrincipalComponents");
		exit(1);
	}

	for (i = 0; i < num_slaves; i++) {
		IPs[i] = GenerateNextIPPayload(NumPats, NumIPs, NumOPs, IPData, OPData, num_pcomponents);

		pvm_send(task_ids[i], MSG_ID);
	}

	printf("%3c %40s %7s %7s %7s\n", '#', "IPs", "MIN", "AVE", "MAX");

	i = 0;

	while (completed_slaves < total_slaves) {
		int	s_tid, s_msg_id, bytes, buff_id;

		float	MinErr, AveErr, MaxErr;

		buff_id = pvm_recv(task_ids[i], MSG_ID);

		if (buff_id < 0) {
			pvm_perror((char *) "ShowErrors");
			exit(1);
		}

		pvm_bufinfo(buff_id, &bytes, &s_msg_id, &s_tid);

		pvm_upkfloat(&MinErr, 1, 1);
		pvm_upkfloat(&AveErr, 1, 1);
		pvm_upkfloat(&MaxErr, 1, 1);

		printf("%3d %40s %7.5f %7.5f %7.5f\n", completed_slaves, IPs[i], MinErr, AveErr, MaxErr);

		free(IPs[i]);

		if (completed_slaves < total_slaves - max_slaves) {
			info = pvm_spawn((char *)"slave", (char**) 0, PvmTaskDefault, (char *)"", 1, task_ids + i);

			if (info != 1) {
				pvm_perror((char *) "DiscoverPrincipalComponents");
				exit(1);
			}

			IPs[i] = GenerateNextIPPayload(NumPats, NumIPs, NumOPs, IPData, OPData, num_pcomponents);

			pvm_send(task_ids[i], MSG_ID);
		}

		completed_slaves++;

		if (++i == num_slaves) {
			i = 0;
		}
	}

	free(task_ids);
}

int
main(int argc, char **argv) {
	int	i, j, NumIPs, NumOPs, NumPats, menu_choice;

	char	Line[LINE_LEN];

	FILE	*file = NULL;

	float **IPData = NULL;
	float **OPData = NULL;

	printf("****************************************\n\tPVM NEURAL NETWORK\n****************************************\n");
	printf("Enter DATA filename: ");

	scanf("%s", Line);

	if ((file = fopen(Line, "r"))) {
		do {
			fgets(Line, LINE_LEN - 1, file);
		} while (*Line == ';'); // eat comments

		sscanf(Line, "%d%d%d", &NumIPs, &NumOPs, &NumPats); // read data specs

		if(NumIPs < 1 || NumIPs > MAXN || NumOPs < 1 || NumOPs > MAXN || NumPats < 1 || NumPats > MAXPATS) {
			printf("NumIPs, NumOPs or NumPats invalid in data file!\n");
			exit(1);
		} else {
			printf("\nThe data has %d patterns comprised of %d inputs and %d outputs.\n", NumPats, NumIPs, NumOPs);
		}

		IPData = Aloc2DAry(NumPats, NumIPs);
		OPData = Aloc2DAry(NumPats, NumOPs);

		for(i = 0; i < NumPats; i++){
			for(j = 0; j < NumIPs; j++) {
				fscanf(file, "%f", &IPData[i][j]);
			}

			for(j = 0; j < NumOPs; j++) {
				fscanf(file, "%f", &OPData[i][j]);
			}
		}

		fclose(file);

		printf("\nPlease select one option:\n  1 => Train neural network\n  2 => Discover principal components\nEnter option: ");

		scanf("%d", &menu_choice);

		switch (menu_choice) {
			case	1:	TrainNeuralNetwork(NumPats, NumIPs, NumOPs, IPData, OPData);
					break;

			case	2:	DiscoverPrincipalComponents(NumPats, NumIPs, NumOPs, IPData, OPData);
					break;
		}

		Free2DAry(IPData, NumIPs);
		Free2DAry(OPData, NumOPs);

		pvm_exit();
	} else {
		perror(Line);
	}

	return(0);
}
