#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *element;
typedef struct ListNode {
  element word;
  element mean;
  struct ListNode *llink;
  struct ListNode *rlink;
} ListNode;

void insert(ListNode **phead, ListNode *new_node);
char *search(ListNode *phead, char *input);
void print_node(ListNode *phead);
void free_list(ListNode *phead);

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

int main() {
  FILE *file = fopen("randdict_utf8.TXT", "r");
  if (file == NULL) {
    perror("파일을 열 수 없습니다.");
    return 1;
  }

  ListNode *phead = NULL;

  char buffer[100];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    // 줄 끝의 개행 문자 제거
    buffer[strcspn(buffer, "\n")] = '\0';

    // `:`을 기준으로 문자열 분리
    char *token = strtok(buffer, ":");
    if (token != NULL) {
      ListNode *new = (ListNode *)malloc(sizeof(ListNode));
      new->word = strdup(trim(token));
      token = strtok(NULL, ":");
      if (token != NULL) {
        new->mean = strdup(trim(token));
      } else {
        new->mean = strdup("");
      }
      new->llink = NULL;
      new->rlink = NULL;
      insert(&phead, new);
    }
  }

  fclose(file);

  // 모든 노드 출력
  print_node(phead);

  int a = 1;
  // 사용자가 입력한 단어의 뜻 출력
  while (a == 1) {
    char input[100];
    printf("검색할 단어를 입력하세요 : ");
    scanf("%99s", input);
    char *trimmed_input = trim(input);

    char *meaning = search(phead, trimmed_input);
    if (meaning != NULL) {
      printf("%s\n", meaning);
      free(meaning); // 의미 문자열의 복사본 해제
    } else {
      printf("단어를 찾을 수 없습니다.\n");
    }
    printf("계속하려면 1을 입력하세요(그만하려면 0을 입력하세요) : ");
    scanf("%d", &a);
  }

  free_list(phead);
  return 0;
}

// 단어 노드 저장 함수
void insert(ListNode **phead, ListNode *new_node) {
  // 첫 노드 삽입
  if (*phead == NULL) {
    *phead = new_node;
  } else {
    ListNode *current = *phead;
    ListNode *prev = NULL;

    // 새로운 노드가 첫 노드보다 앞서야 하는 경우 처리
    if (strcmp(new_node->word, (*phead)->word) < 0) {
      new_node->rlink = *phead;
      (*phead)->llink = new_node;
      *phead = new_node;
      return;
    }

    while (current != NULL && strcmp(current->word, new_node->word) < 0) {
      prev = current;
      current = current->rlink;
    }

    if (prev == NULL) {
      new_node->rlink = *phead;
      (*phead)->llink = new_node;
      *phead = new_node;
    } else {
      new_node->rlink = current;
      new_node->llink = prev;
      if (current != NULL) {
        current->llink = new_node;
      }
      prev->rlink = new_node;
    }
  }
}

void print_node(ListNode *phead) {
  ListNode *current = phead;
  while (current != NULL) {
    printf("%s : %s\n", current->word, current->mean);
    current = current->rlink;
  }
}

char *search(ListNode *phead, char *input) {
  ListNode *current = phead;

  while (current != NULL) {
    if (strcmp(current->word, input) == 0) {
      return strdup(current->mean); // 의미 문자열의 복사본 반환
    }
    current = current->rlink;
  }
  return NULL;
}

void free_list(ListNode *phead) {
  ListNode *current = phead;
  while (current != NULL) {
    ListNode *temp = current;
    current = current->rlink;
    free(temp->word);
    free(temp->mean);
    free(temp);
  }
}
