#include <bits/stdc++.h>
using namespace std;

struct Pedido{
	vector<pair<int, int>> itens;
	int id;		//indice do pedido/corredor
	int qtd;	//quantidade total de itens
	bool usado; //se o pedido/corredor ja foi selecionado
};
typedef Pedido Corredor;

vector<Pedido> pedidos;
vector<Corredor> corredores;
vector<int> pedidos_wave;
vector<int> corredores_wave;

int LB, UB;     // limites
int N, K, M; 	// N #pedidos, K #itens, M #corredores
int total=0;		// total de itens selecionados na wave

bool ordena(Pedido A, Pedido B){return A.qtd>B.qtd;}

bool pedido_viavel(Pedido p, Corredor c){ // checa se um corredor c pode atender um pedido p

	if(total+p.qtd>UB) return false;
	if(p.usado) return false;

	for(int i=0; i<p.itens.size(); i++){
		bool item_achado=false;
		for(int j=0; j<c.itens.size(); j++){
			if(p.itens[i].first == c.itens[j].first){
				item_achado = true;
				if(p.itens[i].second>c.itens[j].second) return false;
			}
		}
		if(!item_achado) return false;
	}
	return true;
}

void remove_itens(Pedido p, Corredor c){ // subtrai do corredor c os itens do pedido p
	for(int i=0; i<p.itens.size(); i++){
		for(int j=0; j<c.itens.size(); j++){
			if(p.itens[i].first==c.itens[j].first){
				c.itens[j].second-=p.itens[i].second;
				c.qtd-=p.itens[i].second;
			}
		}
	}
}

void seleciona_pedidos(){

	sort(pedidos.begin(), pedidos.end(), ordena);			//ordena pedidos

	while(total<=UB){ //procura maximizar uma wave viavel

		sort(corredores.begin(), corredores.end(), ordena); 	//ordena corredores

		int ok=false;

		for(int j=0; j<corredores.size(); j++){
			for(int i=0; i<pedidos.size(); i++){

				if(pedido_viavel(pedidos[i], corredores[j])){

					remove_itens(pedidos[i], corredores[j]);
					total+=pedidos[i].qtd;
					pedidos[i].qtd=-1;

					pedidos_wave.push_back(pedidos[i].id);
					if(!corredores[j].usado) corredores_wave.push_back(corredores[j].id);

					cout<<"o pedido "<<pedidos[i].id<<" foi retirado do corredor "<<corredores[j].id<<" ["<<j<<"]"<<"\n";
					
					ok=true;
					pedidos[i].usado=true;
					corredores[j].usado=true;
				}
			}
		}
		if(ok==false){
			cout<<"NENHUM NOVO PEDIDO VIAVEL\n";
			return;	
		}
	}
}

int main(){

	ifstream input("entrada.txt");
	//ifstream input("entrada2.txt");
	//ifstream input("instance_0001.txt");
	ofstream output("saida.txt");

	input>>N>>K>>M;

	//coleta dos dados de entrada
	for(int i=0; i<N; i++){
		int x;
		input>>x;
		pedidos.push_back(Pedido{{}, i, 0, false});

		for(int j=0; j<x; j++){
			int a, b;
			input>>a>>b;
			pedidos[i].itens.push_back({a, b});
			pedidos[i].qtd+=b;
		}
	}

	for(int i=0; i<M; i++){
		int x;
		input>>x;
		corredores.push_back(Corredor{{}, i, 0, false});

		for(int j=0; j<x; j++){
			int a, b;
			input>>a>>b;
			corredores[i].itens.push_back({a, b});
			corredores[i].qtd+=b;
		}
	}

	input>>LB>>UB; // le limites inferior e superior	

	seleciona_pedidos();
	cout<<"quantidade de itens: "<<total<<"\n";

	output<<pedidos_wave.size()<<"\n";
	for(int i=0; i<pedidos_wave.size(); i++) output<<pedidos_wave[i]<<"\n";
	output<<corredores_wave.size()<<"\n";
	for(int i=0; i<corredores_wave.size(); i++) output<<corredores_wave[i]<<"\n";

	return 0;
}
