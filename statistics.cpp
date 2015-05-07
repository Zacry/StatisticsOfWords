#include<iostream>
#include<string>
using namespace std;
#define HASHLEN 2807303  
#define WORDLEN 30 

typedef struct node_has_space{
	char word[WORDLEN];
	int count;
	struct node_has_space *next;
}node_has_space, *p_node_has_space;

typedef struct node_no_space{
	char *word;
	int count;
	struct node_no_space *next;
}node_no_space, *p_node_no_space;

p_node_no_space bin[HASHLEN] = {NULL};

void swap(int &a, int &b) {
	int temp;
	temp = a;
	a = b;
	b = temp;
}
unsigned int hash(char *p_word) {
	unsigned int index = 0;
	while(*p_word) {
		index += index * 31 + *p_word;
		p_word++;
	}
	return index % HASHLEN;
}

void trim_word(char *word) {
	int n = strlen(word) - 1;
	if(n <= 0)
		return;
	int i = 0;
	while(word[n] < '0' || (word[n] > '9' && word[n] < 'A') || (word[n] > 'Z' && word[n] < 'a') || word[n] > 'z') {
		word[n] = '\0';
		--n;
	}

	while(word[i] < '0' || (word[i] > '9' && word[i] < 'A') || (word[i] > 'Z' && word[i] < 'a') || word[i] > 'z') {
		++i;
	}
	strcpy(word, word + i);
}

void insert_word(char *word) {
	unsigned int index = hash(word);
	node_no_space *p = bin[index];
	while(p) {
		if(strcmp(p->word, word) == 0) {
			(p->count)++;
			return;
		}
		p = p->next;
	}

	p = (node_no_space*)malloc(sizeof(node_no_space));
	p->count = 1;
	p->word = (char*)malloc(strlen(word) + 1);
	strcpy(p->word, word);
	p->next = bin[index];
	bin[index] = p;
}

void write_to_file(char *file_path) {
	FILE* fout = fopen(file_path, "w");
	int i = 0;
	node_no_space *p;
	while(i < HASHLEN) {
		for(p = bin[i]; p != NULL; p = p->next) {
			fprintf(fout, "%s %d\n", p->word, p->count);
		}
		i++;
	}
	fclose(fout);
}

void min_heap(node_has_space heap[], int i, int len) {
	int left = i * 2;
	int right = i * 2 + 1;
	int min_index;

	if(left <= len && heap[left].count < heap[i].count) {
		min_index = left;
	} else {
		min_index = i;
	}

	if(right <= len && heap[right].count < heap[min_index].count) {
		min_index = right;
	}
	if(min_index != i) {
		swap(heap[i].count, heap[min_index].count);
		char buffer[WORDLEN];
		strcpy(buffer, heap[min_index].word);
		strcpy(heap[min_index].word, heap[i].word);
		strcpy(heap[i].word, buffer);
		min_heap(heap, min_index, len);
	}
}

void build_min_heap(node_has_space heap[], int n) {
	int index = n / 2;
	int i;
	for(i = index; i >= 1; i--) {
		min_heap(heap, i, n);
	}
}

void main() {
	int i;
	int _count;
	int n = 10;
	FILE *f_message, *fin;
	char *_word = (char*)malloc(WORDLEN);
	f_message = fopen("string.txt", "r");
	if(!f_message)
		return;
	while(fscanf(f_message, "%s", _word) != EOF) {
		if(strlen(_word)) {
			trim_word(_word);
			insert_word(_word);
		}
	}
	fclose(f_message);

	write_to_file("result.txt");

	fin = fopen("result.txt", "r");
	node_has_space *heap = (node_has_space*) malloc(sizeof(node_has_space) * (n + 1));
	for(i = 1; i <= n; i++) {
		fscanf(fin, "%s %d", _word, &_count);
		heap[i].count = _count;
		strcpy(heap[i].word, _word);
	}
	build_min_heap(heap, n);
	while(fscanf(fin,"%s %d", _word, &_count) != EOF) {
		if(_count > heap[1].count) {
			heap[1].count = _count;
			strcpy(heap[1].word, _word);
			min_heap(heap, 1, n);
		}
	}

	for(int k = 1; k <= n; k++) {
		cout << heap[k].word << ":" << heap[k].count << endl;
	}
}
