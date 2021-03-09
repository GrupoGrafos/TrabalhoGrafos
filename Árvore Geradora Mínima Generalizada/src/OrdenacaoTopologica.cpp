#include "snippet.h"

namespace entities
{
	using BuscaProfundidadeOT = entities::buscaProfundidade::BuscaProfundidadeOT;
	using IBuscaProfundidade = interfaces::IBuscaProfundidade;
	using IGrafo = interfaces::IGrafo;
	using Lista = lista::Lista;

	OrdenacaoTopologica::OrdenacaoTopologica(IGrafo *g)
	{
		this->grafo = g;
		this->buscaProfundidade = new entities::buscaProfundidade->BuscaProfundidadeOT();

	}

	Lista<Vertice*> *OrdenacaoTopologica::Gerar()
	{
		this->buscaProfundidade->Gerar(this->grafo);
		entities::buscaProfundidade->BuscaProfundidadeOT *buscaOT = static_cast<BuscaProfundidadeOT*>(this->buscaProfundidade);

		return buscaOT->ListaOrdemTopologica();

	}

	bool OrdenacaoTopologica::IsUniqueOrdenacaoTopologica()
	{

		entities::buscaProfundidade->BuscaProfundidadeOT *buscaOT = static_cast<BuscaProfundidadeOT*>(this->buscaProfundidade);

		return buscaOT->IsUniqueOrdenacaoTopologica();
	}
}
