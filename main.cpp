using namespace std;
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <time.h>

#define ROWS 1000000
#define COLUMNS 6

class Atrib {
    public:
        int count;
        string nome;

        Atrib() {
            count = 0;
        }
};

class ColecaoAtrib {
    public:
        vector<Atrib> atribs;

        ColecaoAtrib () {
        }

        int searchAtrib (string nome) {
            for(int i=0; i<(atribs.size()); i++) {
                if (atribs[i].nome.compare(nome) == 0) return i;
            }
            return -1;
        }

        void processaAtrib (string nome) {
            int pos = searchAtrib(nome);
            if (pos == -1) {
                atribs.resize(((int) atribs.size())+1);
                atribs[((int) atribs.size()) - 1].nome = nome;
                atribs[((int) atribs.size()) - 1].count++;
                //cout << atribs[((int) atribs.size()) - 1].count << '\n';
            } else {
                atribs[pos].count++;
                //cout << atribs[pos].count << '\n';
            }
        }
};


class Decisao {
    public:
        vector<vector<string> > matriz;
        vector<ColecaoAtrib> colecaoAtribs;
        double entropia;
        int rows;
        int columns;

        Decisao () {
            colecaoAtribs.resize(COLUMNS);
        }

        Decisao (int numCols) {
            colecaoAtribs.resize(numCols);
            columns = colecaoAtribs.size();
        }

        void imprimeDecisao () {
            for(int i=0; i<matriz.size(); i++) {
                for(int j=0; j<matriz[i].size(); j++) {
                     cout << matriz[i][j] << '|';
                }
                cout << '\n';
            }
        }

        Decisao retiraAtributo (int col, string nome, int count) {
            Decisao novaDecisao (colecaoAtribs.size()-1);
            novaDecisao.matriz.resize(count);
            novaDecisao.rows = count;
            for (int i=0; i<novaDecisao.rows; i++) {
                novaDecisao.matriz[i].resize(novaDecisao.colecaoAtribs.size());
            }
            novaDecisao.columns = novaDecisao.colecaoAtribs.size();

            //cout << nome << " " << count << " " << novaDecisao.columns <<  '\n';

            int tempi = 0;
            for (int i=0; i<this->rows; i++) {
                int oldj = 0;
                if (matriz[i][col] == nome) {
                    for (int j=0; j<novaDecisao.columns; j++) {
                        if (j == col) oldj++;
                        novaDecisao.matriz[tempi][j] = matriz[i][oldj];

                        oldj++;
                    }
                    tempi++;
                }
            }

            return novaDecisao;
        }


        void montaDeArquivo (string arquivo) {
            matriz.resize(ROWS);
            for (int i=0; i<ROWS; ++i)
                matriz[i].resize(COLUMNS);
            rows = ROWS;
            columns = COLUMNS;

            fstream myfile ("entrada.txt");

            for(int i=0; i<ROWS; i++) {
                string line;
                getline(myfile, line);

                int aux1 = 0;
                int aux2 = line.find(',', aux1);

                for(int j=0; j<COLUMNS; j++) {
                    matriz[i][j] = line.substr(aux1, aux2-aux1);
                    aux1 = aux2+1;
                    aux2 = line.find(',', aux1);
                }
            }
        }

        void contaProb () {
            for (int i=0; i<this->columns; i++) {
                for (int j=0; j<this->rows; j++) {
                    //cout << "Processando " << matriz[j][i] << ": ";
                    colecaoAtribs[i].processaAtrib(matriz[j][i]);                    
                }
            }
        }

        void calculaEntropiaGeral () {
            contaProb();
            double entropiaPar = 0;
            //cout << "teste " << colecaoAtribs[colecaoAtribs.size()-1].atribs.size() << '\n';
            for (int i=0; i<colecaoAtribs[colecaoAtribs.size()-1].atribs.size(); i++) {
                Atrib * tempAtrib = &colecaoAtribs[colecaoAtribs.size()-1].atribs[i];
                double quociente = (double) (tempAtrib->count)/matriz.size();
                entropiaPar += -((quociente)*(log2(quociente)));
            }
            //cout << "Entropia geral :" << entropiaPar << '\n';
            this->entropia = entropiaPar;
        }

        double calculaGanhoColuna (int col) {
            double ganho = this->entropia;
            for (int i=0; i<colecaoAtribs[col].atribs.size(); i++) {
                Decisao decisaoPar(this->colecaoAtribs.size());
                Atrib * tempAtrib = &colecaoAtribs[col].atribs[i];
                //cout << "Tratando: " << tempAtrib->nome << '\n';
                decisaoPar.matriz.resize(tempAtrib->count);
                decisaoPar.rows = tempAtrib->count;
                for (int j = 0; j < tempAtrib->count; ++j) {
                    decisaoPar.matriz[j].resize(this->columns);
                }
                decisaoPar.columns = this->columns;
                int aux = 0;
                for (int j=0; j<this->rows; j++) {
                    if(this->matriz[j][col] == tempAtrib->nome) {
                        decisaoPar.matriz[aux++] = this->matriz[j];
                    }
                }
                //decisaoPar.imprimeDecisao();
                decisaoPar.calculaEntropiaGeral();
                //cout.precision(3);
                //cout << fixed << decisao.entropia << '\n';
                //cout << "Porcentagem: " << fixed << (double) tempAtrib->count/matriz.size() << " " << decisaoPar.entropia <<'\n';
                ganho -= ((double)tempAtrib->count/matriz.size())*(decisaoPar.entropia);
            }
            //cout << ganho << '\n';
            return ganho;
        }

};

typedef struct No * Apontador;

class No {
    public:
        Decisao decisao;
        vector<Apontador> apontadores;

        No () {
            decisao;
        }

        No * id3 () {
            //cout << "comecei!\n";
            if (this->decisao.columns == 1) {
                cout << this->decisao.matriz[0][0] << '\n';
                return NULL;
            }
            decisao.calculaEntropiaGeral();
            int colParcial = 0;
            double ganhoParcial = 0;
            for (int i=0; i<((decisao.matriz[0].size())-1); i++) {
                double ganho = decisao.calculaGanhoColuna(i);
                if (ganho > ganhoParcial) {
                    colParcial = i;
                    ganhoParcial = ganho;
                }
            }
            //cout.precision(3);
            //cout << fixed << colParcial << " " << ganhoParcial << '\n';

            ColecaoAtrib * colecao = &decisao.colecaoAtribs[colParcial];
            apontadores.resize(colecao->atribs.size());
            //cout << colecao->atribs.size() << "\n";
            for (int i=0; i<apontadores.size(); i++) {
                No no;
                no.decisao = decisao.retiraAtributo(colParcial, colecao->atribs[i].nome, colecao->atribs[i].count);
                //no.decisao.imprimeDecisao();
                apontadores[i] = new No;
                //apontadores[i] = &no;
                apontadores[i] = no.id3();
                //(*apontadores[i]).decisao.imprimeDecisao();
                //else return (*apontadores[i]).id3();
                //cout << "------------------------\n";
            }
            return this;
        }
};

class Arvore {
    public:
        Apontador raiz;
        int profundidade;

        Arvore (int rows, int columns, string arquivo) {
            No raizArvore;
            raizArvore.decisao.montaDeArquivo(arquivo);
            //raizArvore.decisao.imprimeDecisao();
            raiz = new No;
            raiz = raizArvore.id3();
            profundidade = 0;
        }
};


int main() {
    clock_t start, end;
    start = clock();
    Arvore arvore(ROWS, COLUMNS, "entrada.txt");
    //arvore.raiz->id3();
    end = clock();
    cout << "Time required for execution: "
    << (double)(end-start)/CLOCKS_PER_SEC
    << " seconds." << "\n";
}
