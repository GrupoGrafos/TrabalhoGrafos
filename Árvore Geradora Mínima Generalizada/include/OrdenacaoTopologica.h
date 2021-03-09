namespace entities
{

	using IBuscaProfundidade = interfaces::IBuscaProfundidade;
	using IGrafo = interfaces::IGrafo;
	using Lista = lista::Lista;

	class OrdenacaoTopologica
	{

	private:
		IBuscaProfundidade *buscaProfundidade;

		IGrafo *grafo;

	public:
		virtual ~OrdenacaoTopologica()
		{
			delete buscaProfundidade;
			delete grafo;
		}

		OrdenacaoTopologica(IGrafo *g);

		virtual Lista<Vertice*> *Gerar();

		virtual bool IsUniqueOrdenacaoTopologica();

	};
}
