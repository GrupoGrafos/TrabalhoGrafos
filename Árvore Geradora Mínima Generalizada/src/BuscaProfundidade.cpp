#include "snippet.h"

namespace entities::buscaProfundidade
{
	using Aresta = entities::Aresta;
	using IBuscaProfundidade = interfaces::IBuscaProfundidade;
	using IGrafo = interfaces::IGrafo;

	void BuscaProfundidade::Gerar(IGrafo *g)
	{
		this->G = g;

		cor = std::vector<int>(this->G->numeroVertices());
		antecessor = std::vector<int>(this->G->numeroVertices());
		d = std::vector<int>(this->G->numeroVertices());
		f = std::vector<int>(this->G->numeroVertices());

		for (int i = 0; i < this->G->numeroVertices(); i++)
		{
			cor[i] = BRANCO;
			antecessor[i] = std::numeric_limits<int>::max();
			d[i] = -1;
			f[i] = -1;
		}

		time = 0;

		for (int u = 0; u < this->G->numeroVertices(); u++)
		{
			if (cor[u] == BRANCO)
			{
				ExecuteDfs(u);
			}
		}

	}

	void BuscaProfundidade::ExecuteDfs(int u)
	{
		cor[u] = CINZA;
		d[u] = ++time;

		if (!this->G->isFolha(u))
		{

			Aresta *adj = this->G->proximoAdjacente(u);

			while (adj != nullptr)
			{
				int v = adj->getVerticeAdjacente();
				if (cor[v] == BRANCO)
				{
					antecessor[v] = u;
					ExecuteDfs(v);
				}
				adj = this->G->proximoAdjacente(u);
			}

			cor[u] = PRETO;
			f[u] = ++time;

		}

	}

	std::wstring BuscaProfundidade::CaminhoMinimo(int source, int destination)
	{

		if (source == destination)
		{
			caminho += source;
		}
		else if (this->antecessor[destination] == -1)
		{
			caminho = L"Não existe caminho";
		}
		else
		{
			CaminhoMinimo(source, this->antecessor[destination]);
			caminho += L" -> " + std::to_wstring(destination);
		}

		return caminho;
	}
}
