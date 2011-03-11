    
/**
  * EdgeIfc		Interface for Edges 
  * @version	1.0  Date: 15 August 2002
  * @author 	Omar Garcia
**/
public interface EdgeIfc {
     
	 /**
	 * Get the source node of the edge
	 * @return Node 
	 */
	public Node getSource();
	
	/**
	 * Get the sink node of the edge
	 * @return Node 
	 */
	public Node getSink();
} 