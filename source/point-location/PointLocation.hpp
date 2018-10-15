/** @file
 * Implementação do algoritmo de Point Location
 */
#include <vector>

#include "../persistence/RedBlackTree.hpp"


namespace point_location {

/** Um ponto em 2D */
struct Point {
	/** Coordenada X */
	double x;
	/** Coordenada Y */
	double y;

	bool operator < (const Point &o) const;
	bool operator == (const Point &o) const;
	Point operator - (const Point &o) const;
};

typedef std::vector<Point> Polygon;

/**
 * Um segmento em 2D
 * Garantimos que from.x ≤ to.x
 */
struct Segment {
	Segment();
	Segment(Point a, Point b, int pol);
	/** Ponto de origem */
	Point from;
	/** Ponto de fim */
	Point to;

	/* A qual polígono pertence este segmento */
	int polygon;

	/* Se o segmento é da parte "de cima" do polígono */
	bool top;

	/** Comparação de segmentos
	 * @returns Se este segmento está "em baixo" de \p o
	 * @pre Os segmentos devem ser comparáveis, isto é, eles devem se intersectar quando projetados
	 * no eixo x
	 */
	bool operator < (const Segment &o) const;
};

/** Estrutura usada para resolver o problema de Point Location */
class PointLocationSolver {
	public:

	/** Construtor.
	 * Recebe uma lista de polígonos e faz um preprocessamente nestes para conseguir responder
	 * chamadas à #WhichPolygon.
	 * @param polygons Lista de polígonos. Os polígonos devem ser dados em
	 * sentido horário e não se intersectar.
	 */
	PointLocationSolver(std::vector<Polygon> polygons);

	/** Consulta de ponto.
	 * @param p O ponto a ser consultado. Não deve estar na borda de nenhum dos polígono
	 * @returns O indice do polígono que contém o ponto \p p, ou -1 se \p p não está contido em
	 * nenhum dos polígonos.
	 */
	int WhichPolygon(Point p) const;

	private:
	persistence::red_black_tree::RedBlackTree<Segment> rbt;
	std::vector<Point> slabs;
};

} // namespace point_location
