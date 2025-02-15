#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 단어 저장할 배열 선언
typedef struct arr{
  char *word;
  char *mean;
}arr;

void print_Arr(arr* wordArr, int count){
  for(int i=0; i<count; i++){
    printf("%s : %s\n", wordArr[i].word, wordArr[i].mean);
  }
}

// 문자열의 양 끝 공백을 제거하는 함수
char *trim(char *str) {
  char *end;

  // 문자열 앞의 공백 제거
  while (isspace((unsigned char)*str))
    str++;

  if (*str == 0) // 모든 문자가 공백인 경우
    return str;

  // 문자열 뒤의 공백 제거
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // 새로운 널 종단 문자 삽입
  *(end + 1) = 0;

  return str;
}

void selection_sort(arr* wordArr, int count) {
  int i, j, min_idx;
  for (i = 0; i < count - 1; i++) {
    // 최소값의 인덱스를 찾습니다
    min_idx = i;
    for (j = i + 1; j < count; j++) {
      if (strcmp(wordArr[j].word, wordArr[min_idx].word) < 0) {
        min_idx = j;
      }
    }
    // 최소값을 정렬되지 않은 부분의 첫 번째 요소와 교환합니다
    if (min_idx != i) {
      arr temp = wordArr[i];
      wordArr[i] = wordArr[min_idx];
      wordArr[min_idx] = temp;
    }
  }
}

int main() {
  FILE *file = fopen("randdict_utf8.TXT", "r");
  if (file == NULL) {
    perror("파일을 열 수 없습니다.");
    return 1;
  }

  // 순서 없이 배열에 저장 
  arr* wordArr = (arr*)malloc(sizeof(arr) * 50000);
  int count = 0;

  char buffer[100];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    // 줄 끝의 개행 문자 제거
    buffer[strcspn(buffer, "\n")] = '\0';

    // `:`을 기준으로 문자열 분리
    char *token = strtok(buffer, ":");
    if (token != NULL) {
      arr bufArr;
      bufArr.word = strdup(trim(token));
      token = strtok(NULL, ":");
      if (token != NULL) {
        bufArr.mean = strdup(trim(token));
      } else {
        bufArr.mean = strdup("");
      }
      wordArr[count++] = bufArr;
    }
  }

  // 배열 선택 정렬
  selection_sort(wordArr, count);

  // 정렬 후 배열 출력
  printf("\n정렬 후 배열:\n");
  print_Arr(wordArr, count);

  // 동적 할당된 메모리 해제
  for (int i = 0; i < count; i++) {
    free(wordArr[i].word);
    free(wordArr[i].mean);
  }
  free(wordArr);

  fclose(file);

  return 0;

}