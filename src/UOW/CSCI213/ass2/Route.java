/**
	A Route between two Cities
	@version $Revision: 1.8 $
	@author $Author: dezwart $
*/
public class Route extends Edge {
	/**
		Create a new Route between two cities.
		@parameter source The city we are coming from.
		@parameter sink The city we are going to.
		@parameter distance The distance from source to sink.
	*/
	public Route(City source, City sink, int distance) {
		super(source, sink);
		setDistance(distance);
		openRoute();
		source.addRoute(this);
		sink.addRoute(this);
	}

	/**
		Sets the distance between the source and sink.
		@parameter distance The distance between the source and sink.
	*/
	public void setDistance(int distance) {
		this.distance = distance;
	}

	/**
		Gets the distance between the two ends of the Route.
	*/
	public int getDistance() {
		return(distance);
	}

	/**
		Toggles the Route's state to "open".
	*/
	public void openRoute() {
		open = true;
	}

	/**
		Toggles the Route's state to "closed".
	*/
	public void closeRoute() {
		open = false;
	}

	/**
		Print the Route to System.out.
	*/
	public void printRoute() {
		City	city;

		city = (City)getSource();

		System.out.print("Source:\t" + city.getName());

		city = (City)getSink();

		System.out.print("\tSink:\t" + city.getName());
		System.out.print("\tDistance:\t" + distance + "\tState:\t");

		if (open) {
			System.out.println("Open");
		} else {
			System.out.println("Closed");
		}
	}

	/**
		Get the City on the other side of the Route in relation to the City specified.
		@paramater city The city on one end of the link.
	*/
	public City getOtherSide(City city) {
		City	temp;

		temp = (City)getSource();

		if (temp == city) {
			temp = (City)getSink();
		}

		return(temp);
	}

	/**
		Returns the state of the Route
	*/
	public boolean isOpen() {
		return(open);
	}

	private boolean	open;
	private int	distance;
}
