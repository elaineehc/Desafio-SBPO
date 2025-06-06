#include <bits/stdc++.h>
using namespace std;

struct Pedido{
	vector<int> itens;
	int id, qtd;
};
typedef Pedido Corredor;

vector<Pedido> pedidos;
vector<Corredor> corredores;

set<int> corredores_wave;
vector<int> pedidos_wave;

int LB, UB;     // limites
int N, K, M; 	// N #pedidos, K #itens, M #corredores
int total;		// total de itens selecionados na wave

bool ordena(Pedido A, Pedido B){
	return A.qtd>B.qtd;
}

bool pedido_viavel(Pedido p, Corredor c){ // checa se um corredor c pode atender um pedido p

	if(total+p.qtd+c.qtd>UB) return false;

	for(int i=0; i<p.itens.size(); i++){
		if(p.itens[i]>c.itens[i]) return false;
	}
	return true;
}

void remove_itens(Pedido p, Corredor c){ // subtrai do corredor c os itens do pedido p
	for(int i=0; i<p.itens.size(); i++){
		c.itens[i]-=p.itens[i];
	}
}

void seleciona_pedidos(){

	int p_atual=0; // pedido atual
	int c_atual=0; // corredor atual

	while(!pedidos.empty() and total<LB){ //procura maximizar uma wave viavel

		cout<<"p_atual = "<<p_atual<<", c_atual = "<<c_atual<<"\n";

		cout<<"fila atual de pedidos: ";
		for(int i=0; i<pedidos.size(); i++) cout<<pedidos[i].id<<" ";
		cout<<"\n";
		
		if(pedido_viavel(pedidos[p_atual], corredores[c_atual])){

			remove_itens(pedidos[p_atual], corredores[c_atual]);
			total+=pedidos[p_atual].qtd;

			pedidos_wave.push_back(pedidos[p_atual].id);
			corredores_wave.insert(corredores[c_atual].id);

			cout<<"o pedido selecionado foi: "<<pedidos[p_atual].id<<"\n";
			cout<<"quantidade de itens: "<<total<<"\n";

			pedidos.erase(pedidos.begin()+p_atual);

		}else p_atual++;

		if(p_atual==pedidos.size()){
			if(c_atual==corredores.size()) return;
			p_atual=0;
			c_atual++;
		}
	}
}

int main(){

	ifstream input("entrada.txt");
	ofstream output("saida.txt");

	input>>N>>K>>M;

	//coleta dos dados de entrada
	for(int i=0; i<N; i++){
		int x;
		input>>x;
		pedidos.push_back(Pedido{{}, i, 0});
		pedidos[i].itens.resize(K);

		for(int j=0; j<x; j++){
			int a, b;
			input>>a>>b;
			pedidos[i].itens[a]=b;
			pedidos[i].qtd+=b;
		}
	}

	for(int i=0; i<M; i++){
		int x;
		input>>x;
		corredores.push_back(Corredor{{}, i, 0});
		corredores[i].itens.resize(K);

		for(int j=0; j<x; j++){
			int a, b;
			input>>a>>b;
			corredores[i].itens[a]=b;
			corredores[i].qtd+=b;
		}
	}

	input>>LB>>UB; // le limites inferior e superior

	sort(pedidos.begin(), pedidos.end(), ordena);			//ordena pedidos
	sort(corredores.begin(), corredores.end(), ordena); 	//ordena corredores

	seleciona_pedidos();

	output<<pedidos_wave.size()<<"\n";
	for(int i=0; i<pedidos_wave.size(); i++) output<<pedidos_wave[i]<<"\n";
	output<<corredores_wave.size()<<"\n";
	for(set<int>::iterator it=corredores_wave.begin(); it!=corredores_wave.end(); it++) output<<*it<<"\n";

	return 0;
}