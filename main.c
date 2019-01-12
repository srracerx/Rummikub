#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pedrinhas{
	char numero;
	char cor;
	int inteiro;
	struct pedrinhas* prox;
	
}PEDRA;

void cabecalho(){
	system("clear");
	printf("--------------------------------------------------------------------------------");
	printf("            RUMMIKLEBERSON");
	printf("\n--------------------------------------------------------------------------------\n");
	
	return;
}

void inicializa_jogadores(PEDRA** jogadores, int num_jogadores){  //Cria a lista das mãos dos jogadores.
	int i;
	
	for(i = 0; i < num_jogadores; i++){
		jogadores[i] = NULL;
	}
	
	return;
}

void inicializa_pilha(PEDRA** pilha){
	*pilha = NULL;  //Pilha inicialmente vazia.
	
	return;
}

int le_baralho(PEDRA** pilha){  //Lê o baralho do arquivo .txt.
	FILE* arquivo;
	PEDRA* novo;
	int c;
	int flag;
	int num;
	
	num = 0;
	c = 0;
	flag = 0;
	
	arquivo = fopen("baralho.txt", "r");
	
	if(arquivo == NULL){
		cabecalho();
		printf("##ERRO NA ABERTURA DO ARQUIVO!!");
		exit(1);
	}
	
	while(flag != EOF){  //Enquanto não atingir o fim do arquivo. (--Cuidado, pois o getc não ignora o '\n').
		fseek(arquivo, c, SEEK_SET);
		
		novo = (PEDRA*)calloc(1, sizeof(PEDRA));  //Cria um elemento para ser colocado no fim da lista.
		
		if(novo == NULL){
			cabecalho();
			printf("##ERRO DE MEMÓRIA!!");
			exit(1);
		}
		
		fscanf(arquivo, "%c%c ", &(novo->numero), &(novo->cor));  //Preenche os elementos. (~~O fscanf já consome os whitespace QUANDO LÊ STRING~~) (--Isso não funciona quando o especificador é %c--) (~~quando lê o meio de uma linha, não move o ponteiro do arquivo~~).			
		c = ftell(arquivo);
		flag = getc(arquivo);
		
		novo->prox = *pilha;  //Insere o elemento no começo da lista.
		*pilha = novo;
		num++;
	}
	
	fclose(arquivo);
	
	return num;
}

void embaralha(PEDRA** pilha, int num){  //Passa as pedras para um array; Embaralha o array; Reconstrói a lista na ordem do array.
	PEDRA* percorre;
	PEDRA** aleatorio;
	int i;
	
	percorre = *pilha;
	
	aleatorio = (PEDRA**)calloc(num, sizeof(PEDRA*));  //Aloca dinamicamente o array.
	
	if(aleatorio == NULL){
		cabecalho();
		printf("##ERRO DE ALOCAÇÃO!!");
		getchar();
		exit(1);
	}
	
	for(i = 0; i < num; i++){  //Passa as pedras para o array.
		aleatorio[i] = percorre;
		percorre = percorre->prox;
	}
	
	srand((unsigned)time(NULL));  //Embaralha o array. (MÉTODO BEN PFAFF).
    for (i = 0; i < num - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (num - i) + 1);
        PEDRA* t = aleatorio[j];
        aleatorio[j] = aleatorio[i];
        aleatorio[i] = t;
	}	
	
	*pilha = aleatorio[0];  //Reconectando o array.
	
	for(i = 0; i < num; i++){
		if(i == (num-1)){
			aleatorio[i]->prox = NULL;
			break;
		}
		
		aleatorio[i]->prox = aleatorio[i+1];
	}
	
	return;
}

void inteiro(PEDRA** pilha){  //Converte char para int nos structs.
	PEDRA* percorre;
	
	percorre = *pilha;
	
	while(percorre != NULL){
		percorre->inteiro = strtol(&percorre->numero, NULL, 16);
		
		percorre = percorre->prox;
	}  //Obs: vai ficar com lixo de memória no campo dos inteiros quando for coringa. (vira 0)
	
	return;
}

void deleta_pilha(PEDRA** pilha){  //Deleta apenas o primeiro nó da pilha.
	PEDRA* no;
	
	no = *pilha;
	*pilha = no->prox;
	free(no);
	return;
}

void distribui(PEDRA** pilha, int num_jogadores, PEDRA** jogadores, int p_um){
    int i;
	int j;
	PEDRA* percorre;
	PEDRA* novo;
	
	percorre = *pilha;
	
	for(i = 0; i < num_jogadores; i++){
		j = 0;
		
		while(j < 14){
		   novo = (PEDRA*)calloc(1, sizeof(PEDRA));
			   
		   if(novo == NULL){
			  cabecalho();
			  printf("\n\n##ERRO DE MEMÓRIA!!");
			  getchar();
			  exit(1);
		   }
			   
		   novo->numero = percorre->numero;
		   novo->cor = percorre->cor;
		   novo->inteiro = percorre->inteiro;	   
			   
		   novo->prox = jogadores[p_um - 1];
		   jogadores[p_um - 1] = novo;
			   
		   j++;
		   percorre = percorre->prox;
		   deleta_pilha(pilha);
		}
		if(p_um < num_jogadores){	
		   p_um++;
		}
		else{	//	 p_um >= num_jogadores			
		   p_um = 1;
		}
    }
	
	return;
}

void imprime_pilha(PEDRA* pilha){  //**Apenas para TESTE**
	cabecalho();
	
	printf("\n-->PILHA DE PEDRAS\n");
	
	while(pilha != NULL){
		printf("NUMERO: %d      COR: %c\n", pilha->inteiro, pilha->cor);
		pilha = pilha->prox;
	}
	return;
}

void imprime_mao(PEDRA** jogadores, int num){  //**Apenas para TESTE**
	int i;
	PEDRA* percorre;
	
	cabecalho();
		
	for(i = 0; i < num; i++){
		printf("\n-->MÃO DO JOGADOR %d\n", i+1);
		percorre = jogadores[i];
		
		while(percorre != NULL){
			printf("NUMERO: %d      COR: %c\n", percorre->inteiro, percorre->cor);
			percorre = percorre->prox;
		}
	}
	
	return;
}

void imprime_idv(PEDRA** jogadores, int num){  //Imprime a mão individual.
	PEDRA* percorre;
	
	percorre = jogadores[num - 1];
	
	printf("\n>>SUAS PEDRAS:");
	
	while(percorre != NULL){
		printf(" %c%c", percorre->numero, percorre->cor);
		percorre = percorre->prox;
	}
	return;
}

void imprime_tab(PEDRA* tab){  //Imprime o tabuleiro.
	printf("\n>>TABULEIRO:");
	
	while(tab != NULL){
		if(tab->numero != 'X'){
			printf(" %c%c", tab->numero, tab->cor);
		}
		else{
			printf(" ||");
		}
		
		tab = tab->prox;
	}
	
	return;
}

int jogada(PEDRA** tab, PEDRA** jogadores, int p_um, char* ins, int modo){  //##NOTA DO P: Separar a valida em uma função!
	int numero;
	int contador;
	int ptos = 0;
	int i = 0;
	int j = 0;
	int vale;  //Diz se a jogada foi válida. 
	int grupo = 0;
	int seq = 0;
	int quantos = 0;
	PEDRA* percorre;
	
	while(ins[quantos] != '\n'){
		quantos++;
	}
	
	if(modo == 0){  //Abertura.
		//Valida a jogada.
		//-->Grupo:
		
		while(ins[i] != '\0'){  //Mesmo número, cores diferentes (min 3 / max 4).
			if(ins[i] == ins[0]){  //Certifica a parte do "mesmo número".
				grupo = 1;
			}
			else{                                           
				grupo = 0;
				break;
			}
			i = i + 3;
		}
		
		//Certifica a parte do "cores diferentes".
		if(grupo == 1){
		    for(i = 1; i < quantos; i = i + 3){
		        for(j = i + 3; j < quantos; j = j + 3){
				    if(ins[i] != ins[j]){
					    grupo = 1;
				    }
				    else{
					    grupo = 0;
				    }
			    }
		    }
		}
			
		//-->Sequência:
		//Certifica se têm cores iguais 
		i = 1;
		while(ins[i] != '\0'){
			if(ins[1] == ins[i]){
				seq = 1;
			}
			else{
				seq = 0;
				break;
			}
			i = i + 3;
		}
		
		//Certifica a parte da "sequência numérica": 
		
		i = 0;
		if(seq == 1){
		    while(ins[i] != '\0'){  //Muda o condicional!!
			    if((ins[i+3] == ins[i] + 1) || (ins[i+3] == '\0')){
				    seq = 1;
			    }
			    else{
				    seq = 0;
				    break;
			    }
			    i = i + 3;
		    }						
		}
				
		//-->Por fim:
		
		vale = grupo | seq;
		
		if(vale == 0){
			return vale;  //Se a jogada for inválida.
		}
		
		//Realiza a jogada (mexe nas listas)
		/*while(ins[i] != '\n'){
			percorre = jogadores[p_um - 1];
			contador = 0;
			numero = atoi(&ins[i]);
			
			while(contador < numero){  //Chegar à pedra da instrução (como um vetor?).
			    percorre = percorre->prox;
			}
			
			ptos = ptos + percorre->inteiro;  //Acumula os pontos.
			
			i++;
		} */
		
	}
	else{  //Jogo normal.
		//FODEO
	}
	
	return vale;
}

int main(void){		
//Declaração de variáveis------------------------------------------------	
	int num_jogadores;  //Guarda o número de jogadores que estão participando.
	int i;
	int j;
	int valida;
	int opcao;  //Para usar em menus.
	int opcao2;
	int num;  //Guarda o número de pedras na pilha.
	int p_um;  //Indica qual jogador será o primeiro.
	char ins[70];  //Guarda as instruções da jogada.
	PEDRA** jogadores;  //Trata-se de um vetor de ponteiros. Esse vetor guarda listas encadeadas que corres pondem as mãos de cada jogador.
	PEDRA* pilha;  //Aponta para o primeiro elemento da pilha de pedras.
	PEDRA* tab;  //Aponta para o primeiro elemento da pilha que corresponde ao tabuleiro.
	PEDRA* novo;
	
//Atribuições e funções iniciais------------------------------------------------------------
	inicializa_pilha(&pilha);
	inicializa_pilha(&tab);
	
	for(i = 0; i < 70; i++){
		ins[i] = '\0';
	}
	
//Splash-screen----------------------------------------------------------
	cabecalho();
	printf("\n\n~~Pressione <ENTER> para continuar~~");
	getchar();
		
//Informações iniciais---------------------------------------------------
	cabecalho();
	printf("\n\n>>Insira quantos jogadores participarão (MIN  2 / MAX 4):");
	scanf("%d", &num_jogadores);
	
//Cria a mão dos jogadores-----------------------------------------------	
	jogadores = (PEDRA**)calloc(num_jogadores, sizeof(PEDRA*));
	
	if(jogadores == NULL){
		cabecalho();
		printf("##ERRO DE ALOCAÇÃO!!");
		getchar();
		exit(1);
	}
	
	inicializa_jogadores(jogadores, num_jogadores);  //Inicializa as mãos dos jogadores. 
	
	num = le_baralho(&pilha);  //Função para ler baralho em texto. 
	
	embaralha(&pilha, num);  //Função para embaralhar.
	
	inteiro(&pilha);  //Converte os números em hex na struct para inteiros.
	
//Cada jogador pega uma pedra do monte-----------------------------------
    switch(num_jogadores){  //(--FALTA INCLUIR CASO DO CORINGA--)
		case 2:
			if(pilha->inteiro >= pilha->prox->inteiro){  //Considera-se que o jogador um é o primeiro a retirar uma carta do baralho. 
				p_um = 1;
			}
			else{
				p_um = 2;
			}
			
			break;
			
		case 3:
		    if((pilha->inteiro >= pilha->prox->inteiro) && (pilha->inteiro >= pilha->prox->prox->inteiro)){
				p_um = 1;
			}
			
			if((pilha->prox->inteiro > pilha->inteiro ) && (pilha->prox->inteiro > pilha->prox->prox->inteiro)){
				p_um = 2;
			}
			
			if((pilha->prox->prox->inteiro > pilha->inteiro ) && (pilha->prox->prox->inteiro > pilha->prox->inteiro)){
				p_um = 3;
			}
			
			break;
			
		case 4:
		    if((pilha->inteiro >= pilha->prox->inteiro) && (pilha->inteiro >= pilha->prox->prox->inteiro) && (pilha->inteiro >= pilha->prox->prox->prox->inteiro)){
				p_um = 1;
			}
			
			if((pilha->prox->inteiro > pilha->inteiro ) && (pilha->prox->inteiro > pilha->prox->prox->inteiro) && (pilha->prox->inteiro > pilha->prox->prox->prox->inteiro)){
				p_um = 2;
			}
			
			if((pilha->prox->prox->inteiro > pilha->inteiro ) && (pilha->prox->prox->inteiro > pilha->prox->inteiro) && (pilha->prox->prox->inteiro > pilha->prox->prox->prox->inteiro)){
				p_um = 3;
			}		    
			
			if((pilha->prox->prox->prox->inteiro > pilha->inteiro) && (pilha->prox->prox->prox->inteiro > pilha->prox->inteiro) && (pilha->prox->prox->prox->inteiro > pilha->prox->prox->inteiro)){
				p_um = 4;
			}
			
			break;
			
		default:
			cabecalho();
			printf("\n\n##QUANTIDADE DE JOGADORES INVÁLIDA!!");
			getchar();
			exit(1);
	}
	
	cabecalho();
	printf("\n\n>>O JOGADOR %d começa!", p_um);
	getchar();
	getchar();  //Dois getchar (?)
	
	embaralha(&pilha, num);  //Função para embaralhar (de novo).
	
//14 peças são distribuídas para cada jogador----------------------------
	
	distribui(&pilha, num_jogadores, jogadores, p_um);
	//imprime_mao(jogadores, num_jogadores);  **Somente são usados para teste**
    //imprime_pilha(pilha);
	
//Cada jogador deve jogar uma ou mais séries de no mínimo 30 pontos.-----
//Se não puder fazer isso, ele deve recolher uma pedra e passar a sua vez.

	for(i = 0; i < num_jogadores; i++){
		cabecalho();
		imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
		printf("\n\n>>JOGADOR %d, escolha uma opcao:\n", p_um);
		printf("(1)Baixar pedras\n");
		printf("(2)Comprar pedra\n");
		scanf("%d", &opcao);
		
		valida = 0;	
		
		switch(opcao){
			case 1:
				opcao2 = 0;
				
				while(opcao2 == 0){
					while(valida == 0){
					    cabecalho();
					    imprime_idv(jogadores, p_um);
					    imprime_tab(tab);  
					    printf("\n\n>>JOGADOR %d, escolha as pedras que deseja baixar:\n", p_um);
						
						setbuf(stdin, NULL);
					    fgets(ins, 70, stdin);  //Preenche a string de instruções.
						
					    valida = jogada(&tab, jogadores, p_um, ins, 0);  //~IMPLEMENTAR E VALIDAR A JOGADA~
					    if(valida == 0){
						    cabecalho();
						    printf("\n\n##Essa jogada não foi valida. Tente novamente!\n");
							getchar();
							setbuf(stdin, NULL);
					    }
						
					    for(j = 0; j < 70; j++){  //Limpa string.
		                    ins[j] = '\0';
	                    }
					}
					
					imprime_tab(tab);
					
					printf("\n\n>>Deseja terminar a jogada?\n");
					printf("(1)Sim\n");
					printf("(2)Nao\n");
					scanf("%d", &opcao2);
					
					if(opcao2 == 1){
						opcao2 = 1;
					}
					else{
						opcao2 = 0;
					}
					
				}
				
				break;
			case 2: //Compra e passa a vez.				
				novo = (PEDRA*)calloc(1, sizeof(PEDRA));
				
				if(novo == NULL){
					cabecalho();
					printf("\n##ERRO DE ALOCACAO!\n");
					getchar();
					exit(1);
				}
				
				novo->numero = pilha->numero;
				novo->cor =  pilha->cor;
				novo->inteiro =	 pilha->inteiro;
				
				novo->prox = jogadores[p_um - 1];
		        jogadores[p_um - 1] = novo;
				
				deleta_pilha(&pilha);
					
				break;
			default:  //Se digitou a opção errada.
				exit(1);
				
				break;
		}
		
		
		if(p_um < num_jogadores){	
		   p_um++;
		}
		else{	//	 p_um >= num_jogadores			
		   p_um = 1;
		}
	}
	
//O jogo (loop)----------------------------------------------------------

//-----------------------------------------------------------------------
//Dá free nas listas e exibe mensagem de vitória (FIM).	
	
//-----------------------------------------------------------------------
	
return 0;	
}