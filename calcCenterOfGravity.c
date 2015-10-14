#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char seps[] = ",";// [,]文字による区切り-->CSVファイル

// 配列を動的確保する関数
int** allocArray(int row, int col);

// 動的確保したメモリを開放する関数
void freeArray(int** data, int row);

// 改行コードを除去する関数
void chop_crlf(char* buff);

// 読み込んだファイルの行・列数を取得する関数
void readFileFormat(FILE* fp, int* row, int* col);
// ファイルデータの読み込みをする関数
void readFileData(FILE* fp, int** data);

// データを表示する関数
void dispData(int** data, int row, int col);

// 重心の計算をする関数
void calcCenterOfGravity(int **data, double *pxG, double *pyG, int row, int col);

int main(int argc, char* argv[])
{
    int row,col;
    FILE* fp;
    int** data;
	double xG, yG;
	    
    if ( argc < 2 ) {
		return 0;
	}
    
	fp = fopen(argv[1],"r");
    if ( fp == NULL ) {
        fprintf(stderr,"%s: No such file\n",argv[1]);
        return 0;
    }
    
    readFileFormat(fp,&row,&col);
    printf("File format: col/row = %d/%d\n",col,row);

    data = allocArray(row,col);
    readFileData(fp,data);
    fclose(fp);

    dispData(data,row,col);

	calcCenterOfGravity(data, &xG, &yG, row, col);
	
	printf("\nThis image's center of gravity is\n");
	printf("gx = %f, gy = %f\n", xG, yG);
	
    freeArray(data,row);
    
    return 0;
}

int** allocArray(int row, int col)
{
	int i;
    int** data = (int**)calloc(row,sizeof(int*));
    for (i=0; i<row; i++ ) data[i] = (int*)calloc(col,sizeof(int));
    return data;
}

void freeArray(int** data, int row)
{
	int i;
    for ( i=0; i<row; i++ ) {
		free(data[i]);
	}
    free(data);
}

void chop_crlf(char* buff)
{
	unsigned i;
    for (i=0; i<strlen(buff); i++ ) {
        
		if ( buff[i] == '\r' ) buff[i] = 0;
        if ( buff[i] == '\n' ) buff[i] = 0;
		
    }
}

void readFileFormat(FILE* fp, int* row, int* col)
{
    int data;
    char buf[256], *token;
    
    rewind(fp);
    
	*row = *col = 0;
    while ( fgets(buf,256,fp) ) {
        chop_crlf(buf);
        if ( buf[0] == '#' || strlen(buf) == 0 ) continue;
        if ( *row == 0 ) {
            token = (char*)strtok(buf,seps);
            while ( token != NULL ) { // buf にトークンがなくなるまで繰り返す
                sscanf(token,"%d",&data);
                token = (char*)strtok(NULL,seps); // 次のトークンを取得
                *col += 1;
            }
        }
        *row += 1;
    }
}

void readFileData(FILE* fp, int** data)
{
    char buf[256],*token;
    int row,col;
    
    rewind(fp);
    row = col = 0;
    
	while ( fgets(buf,256,fp) ) {
        chop_crlf(buf);
        
		if ( buf[0] == '#' || strlen(buf) == 0 ) continue;
		
        col = 0;
        token = (char*)strtok(buf,seps);
        
		while ( token != NULL ) { // buf にトークンがなくなるまで繰り返す
            sscanf(token,"%d",&data[row][col]);
            token = (char*)strtok(NULL,seps); // 次のトークンを取得
            col ++;
        }
        row ++;
    }
}

void dispData(int** data, int row, int col)
{
	int i, j;
	
    /* printf("** CSV Data:\n"); */
	printf("\n");
    
	for (i=0; i<row; i++ ) {
        /* printf("row[%d]: ", i); */
        for (j=0; j<col; j++ ) {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }
}

void calcCenterOfGravity (int **data, double *pxG, double *pyG, int row, int col) {

	int count = 0;
	int x, y, i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (data[i][j] == 1) {
				x += i;
				y += j;
				count++;
			}
		}
	}
	
	*pxG = (double) x / count, *pyG = (double) y / count;
}
