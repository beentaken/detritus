  
/**
  * Edge 
  * @version 1.0   Date:15 August 2002
  * @author 	Omar Garcia
**/

public class Edge implements EdgeIfc {
	
	private Node source;
	private Node sink;

	/**
	* Constructor: The two nodes must already exists
	*/
	public Edge(Node sourceNode, Node sinkNode) {
		source = sourceNode;
		sink = sinkNode;
	}

	/**
	* {@link EdgeIfc#getSource() Defined in EdgeIfc }
	 */
	public Node getSource() {
		return (source);
	}
	
	/**
	* {@link EdgeIfc#getSink() Defined in EdgeIfc }
	*/
	public Node getSink() {
		return(sink);
	}
	
} 
