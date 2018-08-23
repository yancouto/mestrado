/** @file
 * Implementação do algoritmo de Point Location
 */
#include <vector>

namespace point_location {

/** Um ponto em 2D */
struct Point {
	/** Coordenada X */
	double x;
	/** Coordenada Y */
	double y;
};

typedef std::vector<Point> Polygon;

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
	 * @param p O ponto a ser consultado.
	 * @returns O indice do polígono que contém o ponto \p p, ou -1 se \p p não está contido em
	 * nenhum dos polígonos.
	 */
	int WhichPolygon(Point p);
};

} // namespace point_location
