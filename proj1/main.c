#include <stdio.h>

int a[4];
int b[32],l[6];

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
	if(l[5] == 32) printf("add $%d, $%d, $%d", l[3], l[1], l[2]);
//	else if(l[5] == 33) printf("addu $%d, $%d, $%d", l[3], l[1], l[2]);
	else if(l[5] == 36) printf("and $%d, $%d, $%d", l[3], l[1], l[2]);
//	else if(l[5] == 26) printf("div $%d, $%d", l[1], l[2]);
//	else if(l[5] == 27) printf("divu $%d, $%d", l[1], l[2]);
//	else if(l[5] == 9) printf("jalr $%d, $%d", l[3], l[1]);
//	else if(l[5] == 8) printf("jr $%d", l[1]);
//	else if(l[5] == 16) printf("mfhi $%d", l[3]);
//	else if(l[5] == 18) printf("mflo $%d", l[3]);
//	else if(l[5] == 17) printf("mthi $%d", l[1]);
//	else if(l[5] == 19) printf("mtlo $%d", l[1]);
//	else if(l[5] == 24) printf("mult $%d, $%d", l[1], l[2]);
//	else if(l[5] == 25) printf("multu $%d, $%d", l[1], l[2]);
//	else if(l[5] == 39) printf("nor $%d, $%d, $%d", l[3], l[1], l[2]);
	else if(l[5] == 37) printf("or $%d, $%d, $%d", l[3], l[1], l[2]);
	else if(l[5] == 0) printf("sll $%d, $%d, %d", l[3], l[2], two1(l[4]));
//	else if(l[5] == 4) printf("sllv $%d, $%d, $%d", l[3], l[2], l[1]);
	else if(l[5] == 42) printf("slt $%d, $%d, $%d", l[3], l[1], l[2]);
//	else if(l[5] == 43) printf("sltu $%d, $%d, $%d", l[3], l[1], l[2]);
//	else if(l[5] == 3) printf("sra $%d, $%d, %d", l[3], l[2], two1(l[4]));
//	else if(l[5] == 7) printf("srav $%d, $%d, $%d", l[3], l[2], l[1]);
//	else if(l[5] == 2) printf("srl $%d, $%d, %d", l[3], l[2], two1(l[4]));
//	else if(l[5] == 6) printf("srlv $%d, $%d, $%d", l[3], l[2], l[1]);
	else if(l[5] == 34) printf("sub $%d, $%d, $%d", l[3], l[1], l[2]);
//	else if(l[5] == 35) printf("subu $%d, $%d, $%d", l[3], l[1], l[2]);
//	else if(l[5] == 12) printf("syscall");
//	else if(l[5] == 38) printf("xor $%d, $%d, $%d", l[3], l[1], l[2]);
	else printf("unknown instruction");
}

void lopen() {
	int imme;
	imme = l[3]*64*32+ l[4]*64 + l[5];
	imme = two2(imme);
	if(l[0] == 8) printf("addi $%d, $%d, %d", l[2], l[1], imme);
//	else if(l[0] == 9) printf("addiu $%d, $%d, %d", l[2], l[1], imme);
	else if(l[0] == 12) printf("andi $%d, $%d, %d 0x%.8x", l[2], l[1], imme);
	else if(l[0] == 4) printf("beq $%d, $%d, %d", l[1], l[2], imme);
	else if(l[0] == 5) printf("bne $%d, $%d, %d", l[1], l[2], imme);
//	else if(l[0] == 32) printf("lb $%d, %d($%d)", l[2], imme, l[1]);
//	else if(l[0] == 36) printf("lbu $%d, %d($%d)", l[2], imme, l[1]);
//	else if(l[0] == 33) printf("lh $%d, %d($%d)", l[2], imme, l[1]);
//	else if(l[0] == 37) printf("lhu $%d, %d($%d)", l[2], imme, l[1]);
	else if(l[0] == 15) printf("lui $%d, 0x%02x", l[2], imme);
	else if(l[0] == 35) printf("lw $%d, %d($%d)", l[2], imme, l[1]);
	else if(l[0] == 13) printf("ori $%d, $%d, %d", l[2],l[1], imme);
//	else if(l[0] == 40) printf("sb $%d, %d($%d)", l[2], imme, l[1]);
	else if(l[0] == 10) printf("slti $%d, $%d, %d", l[2],l[1], imme);
//	else if(l[0] == 11) printf("sltiu $%d, $%d, %d", l[2],l[1], imme);
//	else if(l[0] == 41) printf("sh $%d, %d($%d)", l[2], imme, l[1]);
	else if(l[0] == 43) printf("sw $%d, %d($%d)", l[2], imme, l[1]);
//	else if(l[0] == 14) printf("xori $%d, $%d, %d", l[2],l[1], imme);
	else if(l[0] == 2) printf("j %d", two3(l[1]*1024*64*32 + l[2]*1024*64 + imme));
//	else if(l[0] == 3) printf("jal %d", two3(l[1]*1024*64*32 + l[2]*1024*64 + imme));
	else printf("unknown instruction");
}

void inpri() {
	if(l[0] == 0) {
		l0();
	}
	else {
		lopen();
	}
	printf("\n");
}

int main( int argc, char *argv[] ) {
	FILE *fp;
	fp = fopen(argv[1], "rb");
	
	int i, inst;
	inst = 1;

	while( (a[0] = fgetc(fp)) != EOF ) {
		for(i = 1; i < 4; i++) a[i] = fgetc(fp);
		printf("inst %d: ", inst++);
		for(i = 0; i < 4; i++) printf("%02x", a[i]);
		printf(" ");

	    mb();													

              for(i=0; i<32; i++) {							
      	        if(i!=0 && i%4==0) printf(" ");
	       		printf("%d", b[i]);
	       	}
	       	printf("\n");


	    ml();
//              for(i=0; i<6; i++) printf("%d ", l[i]);
//               printf("\n");
		
		inpri();
	}
        fclose(fp);
        return 0;
}
