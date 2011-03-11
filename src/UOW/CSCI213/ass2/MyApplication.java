/**
	MyApplication does stuff
	@version $Revision: 1.5 $
	@author $Author: dezwart $
*/
public class MyApplication {
	public static void main(String[] args) {
		RoadMap map = new RoadMap();

		map.loadData();

		switch (args.length) {
			case 0:
			case 1: System.out.println("Please enter 2 or more City names");
				break;

			case 2:	System.out.println(map.ShortestPath(args[0], args[1]));
				break;

			default:	System.out.println(map.ShortestPath(args));
					break;
		}
	}
}
