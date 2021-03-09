#include <string>
#include <vector>
#include <limits>

namespace entities::buscaProfundidade
{

	using IBuscaProfundidade = interfaces::IBuscaProfundidade;
	using IGrafo = interfaces::IGrafo;

	class BuscaProfundidade : public IBuscaProfundidade
	{

	private:
		static constexpr signed char BRANCO = 0;
		static constexpr signed char CINZA = 1;
		static constexpr signed char PRETO = 2;

		IGrafo *G;

		std::vector<int> cor;
		std::vector<int> antecessor;
		std::vector<int> d;
		std::vector<int> f;

		int time = 0;

		std::wstring caminho = L"";

	public:
		virtual ~BuscaProfundidade()
		{
			delete G;
		}

		void Gerar(IGrafo *g) override;

	private:
		void ExecuteDfs(int u);

	public:
		std::wstring CaminhoMinimo(int source, int destination) override;

	};
}
