import java.util.*;

/**
	A City
	@version $Revision: 1.5 $
	@author $Author: dezwart $
*/
public class City extends Node {
	/**
		Named constructor for a City.
		@parameter name The name of the City.
		@parameter population The population of the City.
	*/
	public City(String name, int population) {
		super(name);

		routes = new Vector();
		setPopulation(population);
	}

	/**
		Set the name of the City.
		@parameter name The name of the City.
	*/
	public void setName(String name) {
		super.setName(name);
	}

	/**
		Set the population of the City.
		@parameter population The population of the City.
	*/
	public void setPopulation(int population) {
		this.population = population;
	}

	/**
		Get the name of the City.
	*/
	public String getName() {
		return(super.getName());
	}

	/**
		Get the population of the City.
	*/
	public int getPopulation() {
		return(population);
	}

	/**
		Add a route to the Vector of Routes.
		@parameter route The Route to add.
	*/
	public void addRoute(Route route) {
		routes.add(route);
		addEdgeTo(route.getOtherSide(this));
	}

	/**
		Get the number of Routes this City has.
	*/
	public int getNumRoutes() {
		return(routes.size());
	}

	/**
		Get the Route at index i in the Routes Vector.
		@parameter i The Route Vector index.
	*/
	public Route getRoute(int i) {
		return((Route)routes.get(i));
	}

	private int	population;
	private Vector	routes;
}
