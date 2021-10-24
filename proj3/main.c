//2017312848_proj2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int a[4];
int b[32],l[6];
int c[32768][5];
int inst;

int r[32];
int pc;
int m[32768];

int time;
int v1, v2, v3;

int two1(int ori) {
	if(ori >= 16) return ori-32;
	else return ori;
}
int two2(int ori) {
	if(ori >= 1024*32 ) return ori-1024*64;
	else return ori;
}
int two3(int ori) {
	if(ori >= 1024*1024*32 ) return ori-1024*1024*64;
	else return ori;
}

void mb() {                                             //make b
	int i, j, c;
	for(i = 0; i<4; i++) {
		c = a[i];
		for(j = 7; j >= 0; j--) {
			b[j+i*8] = c%2;
			c = c/2;
		}
	}
}
void ml() {                                             //make lay
	int i = 0, j,k;
	l[i] = b[0];
	for(j = 1; j < 6; j++)  l[i] = l[i]*2 + b[j];
	for(i = 1; i < 6; i++) {
		l[i] = b[j];
		j++;
		for(k = 1; k < 5; k++) {
			l[i] = l[i]*2 + b[j];
			j++;
		}
	}
	l[5] = l[5]*2 + b[31];
}

void l0() {
	int temp;
		c[inst][1] = l[3];
		c[inst][2] = l[1];
		c[inst][3] = l[2];
		c[inst][4] = l[1];
	if(l[5] == 32) //printf("add $%d, $%d, $%d", l[3], l[1], l[2]);		
		temp = 1;
	else if(l[5] == 36) //printf("and $%d, $%d, $%d", l[3], l[1], l[2]);
		temp = 3;
	else if(l[5] == 37) //printf("or $%d, $%d, $%d", l[3], l[1], l[2]);
		temp = 4;
	else if(l[5] == 0) {//printf("sll $%d, $%d, %d", l[3], l[2], two1(l[4]));
		temp = 16;
		if(l[1]+l[2]+l[3]+l[4] != 0) temp = -1;
	}
	else if(l[5] == 42) //printf("slt $%d, $%d, $%d", l[3], l[1], l[2]);
		temp = 5;
	else if(l[5] == 34) //printf("sub $%d, $%d, $%d", l[3], l[1], l[2]);
		temp = 2;
	else //printf("unknown instruction");
		temp = -1;
	c[inst][0] = temp;
}

void lopen() {
	c[inst][4] = l[1];
	int imme;
	imme = l[3]*64*32+ l[4]*64 + l[5];
	imme = two2(imme);
	if(l[0] == 8) {//printf("addi $%d, $%d, %d", l[2], l[1], imme);
		c[inst][0] = 6;
		c[inst][1] = l[2];
		c[inst][2] = l[1];
		c[inst][3] = imme;
	}	
	else if(l[0] == 12) {//printf("andi $%d, $%d, %d", l[2], l[1], imme);
		c[inst][0] = 7;
		c[inst][1] = l[2];
		c[inst][2] = l[1];
		c[inst][3] = imme;
	}
	else if(l[0] == 4) {//printf("beq $%d, $%d, %d", l[1], l[2], imme);
		c[inst][0] = 13;
		c[inst][1] = l[1];	
		c[inst][2] = l[2];
		c[inst][3] = imme;
	}
	else if(l[0] == 5) {//printf("bne $%d, $%d, %d", l[1], l[2], imme);
		c[inst][0] = 14;
		c[inst][1] = l[1];	
		c[inst][2] = l[2];
		c[inst][3] = imme;
	}
	else if(l[0] == 15) {//printf("lui $%d, 0x%02x", l[2], imme);
		c[inst][0] = 10;
		c[inst][1] = l[2];
		c[inst][2] = imme;
	}
	else if(l[0] == 35) {//printf("lw $%d, %d($%d)", l[2], imme, l[1]);
		c[inst][0] = 11;
		c[inst][1] = l[2];
		c[inst][2] = imme;
		c[inst][3] = l[1];
	}
	else if(l[0] == 13) {//printf("ori $%d, $%d, %d", l[2],l[1], imme);
		c[inst][0] = 8;
		c[inst][1] = l[2];
		c[inst][2] = l[1];
		c[inst][3] = imme;
	}
	else if(l[0] == 10) {//printf("slti $%d, $%d, %d", l[2],l[1], imme);
		c[inst][0] = 9;
		c[inst][1] = l[2];
		c[inst][2] = l[1];
		c[inst][3] = imme;
	}
	else if(l[0] == 43) {//printf("sw $%d, %d($%d)", l[2], imme, l[1]);
		c[inst][0] = 12;
		c[inst][1] = l[2];
		c[inst][2] = imme;
		c[inst][3] = l[1];
	}
	else if(l[0] == 2) {//printf("j %d", two3(l[1]*1024*64*32 + l[2]*1024*64 + imme));
		c[inst][0] = 15;
		c[inst][1] = two3(l[1]*1024*64*32 + l[2]*1024*64 + imme);
	}	
	else //printf("unknown instruction");
		c[inst][0] = 0;
}

void cmake() {
	if(l[0] == 0) {
		l0();
	}
	else {
		lopen();
	}
}

int pr[8][5];
int sum = 0;

void clear() {
	int i,j;
	for(i=0; i<32768; i++) {
		c[i][0] = -1;
		for(j=1; j<5; j++)
			c[i][j] = 0;
	}
	for(i=0; i<8; i++) for(j=0; j<5; j++) pr[i][j] = 0;
	for(i=0; i<32768; i++) m[i] = 4294967295;
	for(j=0; j<32; j++) r[j]=0;
	pc = 0;
	time = 0;
	sum = 0;
}

void pwb() {
	int c1 = pr[7][0], c2 = pr[7][1], c3 = pr[7][2], c4 = pr[7][3];
	if(c1 == 1) r[c2] = r[c3]+r[c4];
	if(c1 == 2) r[c2] = r[c3]-r[c4];
	if(c1 == 3) r[c2] = r[c3]&r[c4];
	if(c1 == 4) r[c2] = r[c3]|r[c4];
	if(c1 == 5) {
		if(r[c3]<r[c4]) r[c2] = 1;
		else r[c2] = 0;
	}
	if(c1 == 6) r[c2] = r[c3] + c4;
	if(c1 == 7) {
		if(c4 < 0 ) c4 = 16*16*16*16 + c4;
		r[c2] = r[c3]&c4;
	}
	if(c1 == 8) {
		if(c4<0) c4 = 16*16*16*16 + c4;
		r[c2] = r[c3]|c4;
	}
	if(c1 == 9) {
		if(r[c3]<c4) r[c2] = 1;
		else r[c2] = 0;
	}
	if(c1 == 10) r[c2] = c3*16*16*16*16;
	if(c1 == 11) r[c2] = m[(r[c4]+c3-16*16*16*16*16*16*16)/4];
}

void pmem() {
	int i;
	for(i=0; i< 5; i++) pr[6][i] = pr[5][i];
	if(pr[5][0] == 12) {
		m[ (r[pr[5][3]]+pr[5][2]-16*16*16*16*16*16*16)/4] = r[pr[5][1]];
	}
}

void pex() {
	int i;
	for(i=0; i< 5; i++) pr[4][i] = pr[3][i];
}

void pid() {
	int i;
	for(i=0; i< 5; i++) pr[2][i] = pr[1][i];	
	int c1 = pr[1][0], c2 = pr[1][1], c3 = pr[1][2], c4 = pr[1][3];

	if(c1 == 13) 
		if(r[c2] == r[c3]) {
			pc = pc -1 + c4;
			pr[0][0] = 0; pr[0][1] = 0; pr[0][2] = 0; pr[0][3] = 0;
		}
	if(c1 == 14)
		if(r[c2] != r[c3]) {
			pc = pc -1 + c4;
			pr[0][0] = 0; pr[0][1] = 0; pr[0][2] = 0; pr[0][3] = 0;
		}	
}

void pif() {
	int i;
	for(i=0; i< 5; i++) pr[0][i] = c[pc][i];
	pc++;

	if(pr[0][0] == 15) pc = pr[0][1];
}

void pipe() {
	if(time == v1) return;
	pif();
	pid();
	pex();
	pmem();
	pwb();

	if((pr[5][0] == 12)&&(pr[5][1] == pr[7][1])) m[ (r[pr[5][3]]+pr[5][2]-16*16*16*16*16*16*16)/4] = r[pr[5][1]];

	int sum1 = sum<<1, sum2 = (unsigned)sum>>31;
	sum = (sum1 | sum2)^r[pr[2][4]];
	if( (pr[2][0] == 11) &&( ((pr[0][0] < 6)||(pr[0][0] > 11) &&( (pr[0][2] == pr[2][1])||(pr[0][3] == pr[2][1]) ) )|| ( (pr[0][0]>5)&&(pr[0][0] < 11)&&( pr[0][2] == pr[2][1]))) ) {
		pc--;
		pr[0][0] = 0; pr[0][1] = 0; pr[0][2] = 0; pr[0][3] = 0;
	}

	int i,j;
	for(i = 0; i<4; i++) for(j=0; j<5; j++) pr[i*2+1][j] = pr[i*2][j];
	time++;
	pipe();
}


int main( int argc, char *argv[] ) {
	FILE *fp;
	fp = fopen(argv[1], "rb");
		
	
	int i;
	inst = 0;
	clear();

	while( (a[0] = fgetc(fp)) != EOF ) {
		for(i = 1; i < 4; i++) a[i] = fgetc(fp);
	    mb();													
	    ml();
		cmake();
		inst++;
	}

	v1 = atoi(argv[2]);
	pipe();
	pc--;
	
	if( argc <= 3) {
		fclose(fp);
		return 0;
	}
	if( strcmp(argv[3], "reg")==0) {
		printf("Checksum: 0x%.8x\n", sum);
		for(i=0; i<32; i++) printf("$%d: 0x%.8x\n", i, r[i]);
		printf("PC: 0x%.8x\n", pc*4);
		fclose(fp);
		return 0;
	}
	if( strcmp(argv[3], "mem")==0) {
		int mem = strtol(argv[4], NULL, 16)-16*16*16*16*16*16*16;
		mem = mem/4;
		int fin = atoi(argv[5]);
		for(i=0; i< fin; i++) printf("0x%.8x\n", m[mem+i]);
		fclose(fp);
		return 0;
	}
		
	fclose(fp);
    return 0;
}
