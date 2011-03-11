import java.util.*;

/**
	A Road Map
	@version $Revision: 1.24 $
	@author $Author: dezwart $
*/
public class RoadMap extends Graph {
	/**
		Default Constructor for the RoadMap, sets the Graph name to "Australia"
	*/
	public RoadMap() {
		this("Australia");
	}

	/**
		Generic Constructor for the RoadMap
		@parameter name The name of the RoadMap
	*/
	public RoadMap(String name) {
		super(name);
		routes = new Vector();
	}

	/**
		Add a City to the RoadMap. Check for duplicates.
		@parameter cityName The name of the City.
		@parameter population The size of the population.
		@return The constructed City or the existing one in the Vector.
	*/
	public City addCity(String cityName, int population) {
		City	city;

		// Sanity check
		if (population < 0) {
			return(null);
		}

		// Call the predefined nodeNameExist in super to handle
		// Duplicates
		city = (City)nodeNameExist(cityName);
		if (city == null) {
			city = new City(cityName, population);
			listNodes.add(city);
		}

		return(city);
	}

	/**
		Returns the names of all the cities in a vector
	*/
	public Vector GetAllCityNames() {
		return(GetAllCityBiggerThan(CITY_NO_SIZE));
	}

	/**
		Returns a Vector containing the names of all Cities with a population greater <code>thisPopulation</code>.
		@parameter thisPopulation The population a city has to be greater than to make it in to the vector.
	*/
	public Vector GetAllCityBiggerThan(int thisPopulation) {
		int	size = listNodes.size();
		Vector	names = new Vector(size);
		City	city;

		for (int i = 0; i < size; i++) {
			city = (City)listNodes.get(i);

			if (city.getPopulation() > thisPopulation) {
				names.add(city.getName());
			}
		}

		return(names);
	}

	/**
		Adds two routes between two cities, one for each direction.
		@parameter firstCityName The name of the First City.
		@parameter secondCityName The name of the Second City.
		@parameter distance The distance between the First and Second City.
	*/
	public void addRoute(String firstCityName, String secondCityName, int distance) {
		City	first;
		City	second;
		Route	route;

		// As the handling of duplicate cities is handled in the
		// addCity method, we can explicitly add them here.
		first = addCity(firstCityName, 0);
		second = addCity(secondCityName, 0);

		// Add the new route to the Vector of routes
		route = new Route(first, second, distance);
		routes.add(route);
	}

	/**
		Lists all the Routes available.
	*/
	public void showRoutes() {
		int	numRoutes = routes.size();
		Route	route;

		for (int i = 0; i < numRoutes; i++) {
			route = (Route)routes.get(i);
			route.printRoute();
		}
	}

	public void loadData() {
		addRoute("Sydney", "Liverpool",32);
		setPopulation("Sydney", 2500000);
		setPopulation("Liverpool", 300000);      
		addRoute("Liverpool","Mittagong",76);
		addRoute("Mittagong","Wollongong",125);
		addRoute("Mittagong", "Goulburn", 85);
		addRoute("Goulburn", "Yass", 87);
		addRoute("Yass", "Canberra", 60  );
		addRoute("Yass", "Gundagia", 56 );
		addRoute("Yass", "Tarcuta", 146 );
		addRoute("Tarcuta", "Holbrook", 69);
		addRoute("Tarcuta", "Cooma", 233);
		addRoute("Holbrook", "Albury", 68 );
		addRoute("Albury", "Wangaratta", 81 );
		addRoute("Wangaratta", "Melbourne", 235 );
		addRoute("Sydney", "Waterfall", 42);
		addRoute("Waterfall", "Wollongong", 57);
		addRoute("Wollongong", "Nowra", 61 );
		addRoute("Nowra", "Ulladula", 58);
		addRoute("Ulladula", "Batesmans Bay", 58);
		addRoute("Batesmans Bay", "Moruya", 27);
		addRoute("Batesmans Bay", "Camberra", 150);
		addRoute("Moruya", "Narooma", 43 );
		closeRoute ("Moruya", "Narooma");
		addRoute("Narooma", "Merimbula",113);
		addRoute("Narooma", "Cooma",180);
		addRoute("Merimbula", "Genoa",  89);
		addRoute("Genoa", "Lakes Entrance", 172);
		addRoute("Lakes Entrance", "Bairnsdale", 35 );
		addRoute("Bairnsdale", "Sale", 66);
		addRoute("Sale", "Melbourne" , 214);
	}

	/**
		set Method for the population of a City in this RoadMap.
		@parameter cityName The name of the City to set the population for.
		@parameter popn The population.
	*/
	public void setPopulation(String cityName, int popn) {
		int	foo = getCityIndex(cityName);
		City	city;

		if (foo > -1) {
			city = (City)listNodes.get(foo);
			city.setPopulation(popn);
		}
	}

	/**
		Opens the Route between the named Cities.
		@parameter cityA The name of City A.
		@parameter cityB The name of City B.
	*/
	public void openRoute(String cityA, String cityB) {
		(findRoute(cityA, cityB)).openRoute();
	}

	/**
		Closes the Route between the named Cities.
		@parameter cityA The name of City A.
		@parameter cityB The name of City B.
	*/
	public void closeRoute(String cityA, String cityB) {
		(findRoute(cityA, cityB)).closeRoute();
	}

	/**
		Finds a Route corresponding the the names of two Cities.
		@parameter cityA The name of City A.
		@parameter cityB The name of City B.
		@return the Route between City A & B or null.
	*/
	public Route findRoute(String cityA, String cityB) {
		Route	route = null;

		City	source, sink;

		String	sourceName, sinkName;

		int	numRoutes = routes.size();
		int	i;

		for (i = 0; i < numRoutes; i++) {
			route = (Route)routes.get(i);

			source = (City)route.getSource();
			sink = (City)route.getSink();

			sourceName = source.getName();
			sinkName = sink.getName();

			if ((sourceName.equals(cityA) && sinkName.equals(cityB)) || (sourceName.equals(cityB) && sinkName.equals(cityA))) {
				break;
			}
		}
	
		return((i < numRoutes) ? route : null);
	}

		
	/**
		Find the Shortest Path between two Cities.
		@parameter originCity The City that we start from.
		@parameter destinationCity The City that we end at.
		@return A Vector containing the Names of all the Cities traversed in order.
	*/
	public Vector ShortestPath(String originCity, String destinationCity) {
		int		oCity = getCityIndex(originCity);
		int		dCity = getCityIndex(destinationCity);

		genLengths();

		if (oCity > -1 && dCity > -1) {
			return(Dijkstra(oCity, dCity));
		} else {
			return(new Vector());
		}
	}

	/**
		Fudged travelling salesman method.
		@parameter Cities An array of City Names to be visited.
		@return A Vector containing the Names of all the Cities traversed in order.
	*/
	public Vector ShortestPath(String [] Cities) {
		Vector	path = new Vector();

		int	cityPairLength = Cities.length - 1;
		int	i;

		// Add path to itself to avoid a java.lang.ArrayIndexOutOfBoundsException
		path.add(path);

		for (i = 0; i < cityPairLength; i++) {
			path.remove(path.size() - 1);
			path.addAll(ShortestPath(Cities[i], Cities[i + 1]));
		}

		// Head back to the Home city
		path.remove(path.size() - 1);
		path.addAll(ShortestPath(Cities[i], Cities[0]));

		return(path);
	}

	/**
		Get the Vector index of a named City.
		@parameter cityName The name of the City.
		@return the index of the named City in the Vector of Cities or -1.
	*/
	private int getCityIndex(String cityName) {
		int	i, numCities = listNodes.size();

		City	city;

		String	name;

		for (i = 0; i < numCities; i++) {
			city = (City)listNodes.get(i);
			name = city.getName();

			if (name.equals(cityName)) break;
		}

		return((i < numCities) ? i : -1);
	}

	private void genLengths() {
		int		num_routes = routes.size();
		int		num_cities = listNodes.size();
		int		i, j, city_routes, index;

		City		city, other_end;
		Route		route;

		lengths = new int[num_cities][num_cities];

		for (i = 0; i < num_cities; i++) {
			for (j = 0; j < num_cities; j++) {
				lengths[i][j] = -1;
			}
		}

		for (i = 0; i < num_cities; i++) {
			city = (City)listNodes.get(i);
			city_routes = city.getNumRoutes();

			for(j = 0; j < city_routes; j++) {
				route = city.getRoute(j);
				
				other_end = route.getOtherSide(city);
				index = listNodes.indexOf(other_end);

				if (route.isOpen()) {
					lengths[i][index] = route.getDistance();
				}
			}
		}
	}

		
	private Vector			routes;
	private static final int	CITY_NO_SIZE = -1;
	private int [][]		lengths;

	/**
		Dijkstra pathing code.
		@parameter src The source node.
		@parameter dest The destination node.
		@return A Vector containing the Names of all the Cities traversed in order from Source to Destination.
	*/
	private Vector Dijkstra(int src, int dest) {
		int		num_nodes = listNodes.size();
		int []		dist;
		int []		paths;

		boolean []	notInSoln;

		dist = new int[num_nodes];
		paths = new int[num_nodes];
		notInSoln = new boolean[num_nodes];

		int	i, j, w, v;

		/* Initalize the nodes array */
		for (i = 0; i < num_nodes; i++) {
			if (i == src) {
				notInSoln[i] = false;	/* notInSoln[i] E of the set S */
			} else {
				notInSoln[i] = true;	/* notInSoln[i] E of the set C */
			}
		}

		/* Initalize the paths array and distances vector */
		for (i = 0; i < num_nodes; i++) {
			dist[i] = lengths[src][i];
			paths[i] = src;
		}

		paths[src] = -1;

		for (i = 1; (i < num_nodes) && notInSoln[dest]; i++) {
			/* Reset v */
			v = -1;

			/* Find the shortest edge length in the distances vector to a node that is not in the solution */
			for (j = 0; j < num_nodes; j++) {
				if (notInSoln[j] && (dist[j] > -1)) {
					if (v > -1) {
						if (dist[v] > dist[j]) {
							v = j;
						}
					} else {
						v = j;
					}
				}
			}

			notInSoln[v] = false;	/* remove this node from C and place in to S */

			/* for each node w E C */
			for (w = 0; w < num_nodes; w++) {
				if (notInSoln[w]) {
					if (dist[w] > -1) {
						if ((lengths[v][w] > -1) && ((dist[v] + lengths[v][w]) < dist[w])) {
							dist[w] = dist[v] + lengths[v][w];
							paths[w] = v;
						}
					/* Check to see if there now a edge between v and w */
					} else if (lengths[v][w] > -1) {
						dist[w] = dist[v] + lengths[v][w];
						paths[w] = v;
					}
				}
			}
		}

		/* Translate the paths[] to a Vector of Cities */
		i = dest;

		Vector	traversed = new Vector();

		City	city;

		while (i > -1) {
			city = (City)listNodes.get(i);
			traversed.insertElementAt(city.getName(), 0);
			i = paths[i];
		}

		return(traversed);
	}
}
