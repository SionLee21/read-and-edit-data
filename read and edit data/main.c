#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct st_data{
	char name[50];	// 지역명
	int family;		// 한국인 세대수
	int korean;		// 한국인 수
	int foreign;	// 외국인 수
	int total;		// 지역별 인구수 총합
	float fo_ratio;	// 지역별 외국인 비율
	float fa_ratio;	// 지역별 한국인 세대당 인구수
}st_data;

// data.txt 파일 구조
// 지역이름 한국인세대수 한국인수 외국인수

int readFile(st_data* p[], char filename[20]); // 데이터 읽기
void listData(st_data* p[], int size); // 전체 목록 출력
void findName(st_data* p[], int size); // 지역명 찾기
int addData(st_data* p[], int size); // 목록 추가
void editData(st_data* p[], int size); // 목록 수정
void statData(st_data* p[], int size); // 통계 보기
void ranking(st_data* p[], int size); // 순위 보기
void saveFile(st_data* p[], int size, char filename[20]); // 데이터 저장
void total(st_data* p[], int size); //총합을 구하기
void evalData(st_data* p); // 구조체 데이터 내용 계산하기
void printData(st_data* p); // 데이터 출력
int askMenu(); // 메뉴 출력
st_data* getMaxData(st_data* p[], int size, int kind); // 특정 멤버가 가장 큰 값을 가진 데이터 찾기
st_data* getMinData(st_data* p[], int size, int kind); // 특정 멤버가 가장 작은 값을 가진 데이터 찾기

int main(void) {
	st_data* data[20];
	int count;
	int menu, quit=0, edited=0;

	srand(time(0));
	count = readFile(data, "data.txt");
	printf("%d개 지역의 정보를 읽었습니다!\n", count);

	while(quit==0){
		menu = askMenu();
		switch(menu){
			case 1: listData(data, count); break;
			case 2: count = addData(data, count); edited=1; break;
			case 3: editData(data, count); edited=1; break;
			case 4: findName(data, count); break;
			case 5: statData(data, count); break;
			case 6: ranking(data, count); break;
      case 7: total(data, count); break;
			case 0: quit = 1; break;
			default : quit = 1;
		}
	}
	if (edited==1) {
		int yesno;
		printf("데이터 파일을 저장할까요? (1 Yes 0 No) > ");
		scanf("%d", &yesno);
		if(yesno == 1) saveFile(data, count, "data.txt");
	}
	return 0;
}

int readFile(st_data* p[], char filename[20]){
	int i=0;
	FILE* file;
	file = fopen(filename, "r");
	while(!feof(file)){
		char name[50];
		int n1, n2, n3;
		int size = fscanf(file, "%s %d %d %d", name, &n1, &n2, &n3);
		if(size<4) break;
		p[i]=(st_data*)malloc(sizeof(st_data));
		strcpy(p[i]->name, name);
		p[i]->family = n1; 
		p[i]->korean = n2;
		p[i]->foreign = n3;
		evalData(p[i]);
		i++;
	}
	fclose(file);
	return i; //이것은 count의 값이 된다
}

void saveFile(st_data* p[], int size, char filename[20]){
//for문을 통하여 size값 만큼 진행하여 데이터 파일을 열고 현재 포인터 구조체에 있는 데이터들을 데이터 파일에 fprintf로 적어주면 됩니다. 
  int i=0;
  FILE *file;
  file = fopen(filename, "wt");
  for (i=0; i<size; i++){
    fprintf(file, "%s %d %d %d\n",p[i]->name,p[i]->family,p[i]->korean,p[i]->foreign);
  }
  fclose(file);
  printf("%d개 자료가 저장되엇습니다.", size);
}

void listData(st_data* p[], int size){
	printf("전체 자료 (%d개 지역)\n", size);
	for(int i=0; i<size; i++)
		printData(p[i]);
}

int addData(st_data* p[], int size){
	st_data* n;
	char name[50];
	int n1, n2, n3;
	printf("새로운 자료 입력\n");
	printf(" > 지역 이름 : ");
	scanf("%s", name);
	printf(" > 한국인 세대수 : ");
	scanf("%d", &n1);
	printf(" > 한국인 수 : ");
	scanf("%d", &n2);
	printf(" > 외국인 수 : ");
	scanf("%d", &n3); //먼저 p의 size배열에 메모리를 할당해주어야 한다. 그리고 나서 위에 저장해둔 데이터를 새롭게 할당한 p의 배열에 넣어주면 되고. 넣어준후에는 해당 지역의 정보를 추가했다고 출력한다. 그리고 나서는 size의 값을 하나 늘려주어야 이 전체 프로그램에서 데이터 즉 count의 값이 하나 더 생겼다고 인식한다. 
  p[size]=(st_data*)malloc(sizeof(st_data));
  strcpy(p[size]->name,name);
  p[size]->family=n1;
  p[size]->korean=n2;
  p[size]->foreign=n3;
  evalData(p[size]);
  
	return size+1;
}

void editData(st_data* p[], int size){
	st_data* n;
	char name[50];
	int count=0, no;
	int n1, n2, n3;
	printf("자료 수정\n");
	printf(" > 지역 이름은? > ");
	scanf("%s", name); //사용자에게 어떠한 단어를 포함하고 있는 지역을 변경할건지 물어보고 해당 단어가 포함되어 있는 지역의 번호와 이름을 순서대로 보여주면서 어떠한 번호의 지역을 수정할 것이지 물어보고 이때 i가 +1이 된 상태이니 no는 -1이 되어야 해야 될 것입니다. 이렇게 변경할 내용를 입력 받고 마지막에 계산하는 함수를 넣어주면 데이터를 수정하는 함수 또한 마무리
  for(int i=0; i<size; i++){
    if (strstr(p[i]->name, name)){
      count++;
      printf("[%d] ", i+1);
      printData(p[i]);
    }
  }
  printf("위 목록에서 수정하려는 지역 번호를 입력하시오 > ");
  scanf("%d", &no);
  no-=1;
  printData(p[no]);
  printf("변경할 내용을 입력하시오.\n");
  printf(" > 한국인 세대수 : ");
  scanf("%d", &p[no]->family);
  printf(" > 한국인 수 : ");
  scanf("%d", &p[no]->korean);
  printf(" > 외국인 수 : ");
  scanf("%d", &p[no]->foreign);
  evalData(p[no]);
	printf("수정되었습니다.\n");
}

void findName(st_data* p[], int size){
	char name[50];
	int count=0;
	printf("Enter a name > ");
	scanf("%s", name);
	for(int i=0; i<size; i++){
		if(strstr(p[i]->name, name)){
			count++;
			printData(p[i]);	
		}
	}
	printf("%d개의 자료를 찾았습니다.\n", count);
}

void statData(st_data* p[], int size){

	st_data* s;
	printf("1) 가장 많은 인구가 사는 지역\n");
	s = getMaxData(p, size, 4);
	printData(s);

	printf("2) 가장 적은 인구가 사는 지역\n");
	s = getMinData(p, size, 4);
	printData(s);
	
	printf("3) 외국인 비율이 가장 높은 지역\n");
	s = getMaxData(p, size, 5);
	printData(s);

	printf("4) 외국인 비율이 가장 낮은 지역\n");
	s = getMinData(p, size, 5);
	printData(s);
	
	printf("5) 세대당 가장 많은 사람이 사는 지역\n");
	s = getMaxData(p, size, 6);
	printData(s);

	printf("6) 세대당 가장 적은 인구가 사는 지역\n");
	s = getMinData(p, size, 6);
	printData(s);
}

void ranking(st_data* p[], int size){
  st_data* temp;
	st_data* rank[20];
	int kind, order;
  temp=(st_data*)malloc(sizeof(st_data));
	char kinds[3][50] = {"총인구 수", "외국인 비율", "세대당 인구 수"};
	char orders[2][50] = {"상위 3개", "하위 3개"};
	printf("원하는 랭킹 항목은? (1: 총인구 수, 2: 외국인 비율, 3: 세대당 인구 수) > ");
	scanf("%d", &kind);
	printf("원하는 랭킹 정렬은? (1: 상위 3개, 2: 하위 3개) > ");
	scanf("%d", &order);
//여기에 rank가 3개의 배열을 가지고 있지만 본인은 20개로 할당하도록 함. 여기에 있는 새로운 포인터 구조체에 for문으로 메모리 할당해주고 rank구조체에 p구조체 있는 모든 데이터들을 받아와서 bubble sort로 kind값에 해당하는 항목에 대해서 비교하여 순서대로 정렬하고 order의 값을 통해 1이면 앞에서 부터 3개를 출력하고 else 뒤에서 부터 3개를 출력하도록 한다. 
  for (int i=0; i<size; i++){
    rank[i]=(st_data*)malloc(sizeof(st_data));
    rank[i]=p[i];
  }
  for (int i=size-1; i>=0; i--){
    for(int j=0; j<i; j++){
      if (kind==1 && rank[j]->total<rank[j+1]->total){
        temp=rank[j];
        rank[j]=rank[j+1];
        rank[j+1]=temp;
      }
      else if (kind==2 && rank[j]->fo_ratio<rank[j+1]->total){
        temp=rank[j];
        rank[j]=rank[j+1];
        rank[j+1]=temp;
      }
      else if (kind==3 && rank[j]->fa_ratio<rank[j+1]->total){
        temp=rank[j];
        rank[j]=rank[j+1];
        rank[j+1]=temp;
      }
    }
  }
  if(order==1){
    for (int i=0; i<3; i++){
      printf("%d) ",i+1);
      printData(rank[i]);
    }
  }
  else{
    for (int i=size-1, j=1; i>size-4; i--,j++){
      printf("%d) ", j);
      printData(rank[i]);
    }
  }
}

int askMenu(){
	int menu;
	printf("\n[ 1:List 2:Add 3:Edit 4:Find 5:Statistics 6:Ranking 7:Total 0:Quit ]\n");
	while(1){
		printf(" > Select a menu > ");
		scanf("%d", &menu);
		if(menu >=0 && menu <=7) break;
		else printf(" > Wrong menu number!\n");
	}
	return menu;
}

void evalData(st_data* p){ 
	p->total = p->korean + p->foreign;
	p->fa_ratio = 1.0*p->korean/p->family;
	p->fo_ratio = 1.0*p->foreign/p->total;
}

void printData(st_data* p){
	printf("%s\n", p->name);
	printf(" > 인구 : %d명 (한국인 %d명, 외국인 %d명)\n", p->total, p->korean, p->foreign);
	printf(" > 외국인 비율 : 1천명 당 %.1f명\n", 1000*p->fo_ratio);
	printf(" > 한국인 세대당 평균 가족 수 : %.3f명 (세대수 %d)\n", p->fa_ratio, p->family);
}

st_data* getMaxData(st_data* p[], int size, int kind){
//kind로 어떤 항목의 max또는 min의 값을 구하는지 알아내고 max나 min값을 p[0]의 해당 항목의 값으로 처음 지정하고 for문으로 p[i]의 값이 max의 값보다 클 경우 해당 i의 값을 j에 저장하고 p[j]를 return한다. 
  int i,j;
  float max;
  if (kind==4){
    max=p[0]->total;
    for (i=0; i<size; i++){
      if (max<p[i]->total){
        max=p[i]->total;
        j=i;
      }
    }
  }
  if (kind==5){
    max=p[0]->fo_ratio;
    for (i=0; i<size; i++){
      if (max<p[i]->fo_ratio){
        max=p[i]->fo_ratio;
        j=i;
      }
    }
  }
  if (kind==6){
    max=p[0]->fa_ratio;
    for (i=0; i<size; i++){
      if (max<p[i]->fa_ratio){
        max=p[i]->fa_ratio;
        j=i;
      }
    }
  }

	return p[j];
}

st_data* getMinData(st_data* p[], int size, int kind){
  int i,j;
  float max;
  if (kind==4){
    max=p[0]->total;
    for (i=0; i<size; i++){
      if (max>p[i]->total){
        max=p[i]->total;
        j=i;
      }
    }
  }
  if (kind==5){
    max=p[0]->fo_ratio;
    for (i=0; i<size; i++){
      if (max>p[i]->fo_ratio){
        max=p[i]->fo_ratio;
        j=i;
      }
    }
  }
  if (kind==6){
    max=p[0]->fa_ratio;
    for (i=0; i<size; i++){
      if (max>p[i]->fa_ratio){
        max=p[i]->fa_ratio;
        j=i;
      }
    }
  }
	return p[j];
}
void total(st_data* p[], int size){
  int total[4]={0};
  //for문으로 total이라는 int배열을 만들어 각각의 항목에 p[i]->total과 korean, foreign, family의 값을 +=로 계속해서 더해줍니다. 최종적으로 그것을 출력하게 합니다. 
  for(int i=0; i<size; i++){
    total[0]+=p[i]->total;
    total[1]+=p[i]->korean;
    total[2]+=p[i]->foreign;
    total[3]+=p[i]->family;
  }
  printf("총 %d개 시의 총합 \n 총 인구 수 : %d명 (총 한국인 : %d명, 총 외국인 : %d명)\n 총 세대 수 : %d세대", size,total[0],total[1],total[2],total[3]);
}
