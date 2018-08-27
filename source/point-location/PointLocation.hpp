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

	void Rotate(double angle);
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

	int polygon;

	/** Altura no ponto x
	 * @returns A coordenada y quando a coordenada x é dada
	 * @pre `from.x ≤ x ≤ to.x`
	 * @note Se `from.x = to.x` então `from.y` é devolvido
	 */
	double HeightAtX(double x) const;

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
	 * @param polygons Lista de polígonos. Os polígonos devem ser válidos e não se intersectar.
	 */
	PointLocationSolver(std::vector<Polygon> polygons);

	/** Consulta de ponto.
	 * @param p O ponto a ser consultado. Não deve estar na borda de nenhum poligono
	 * @returns O indice do polígono que contém o ponto \p p, ou -1 se \p p não está contido em
	 * nenhum dos polígonos.
	 */
	int WhichPolygon(Point p) const;

	private:
	persistence::red_black_tree::RedBlackTree<Segment> rbt;
	double angle;
	std::vector<double> slabs;
};

} // namespace point_location
