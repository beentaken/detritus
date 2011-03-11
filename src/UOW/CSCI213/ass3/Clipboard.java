
public class Clipboard {
	private Shape	shape;

	Clipboard() {
		shape = null;
	}

	/**
		Stores a copy of a Shape in this Clipboard.
		@param s The Shape to store a copy of.
	*/
	public void store(Shape s) {
		shape = copy(s);
	}

	/**
		Returns a copy of the Shape in this Clipboard.
		@returns A copy of the Shape in this Clipboard.
	*/
	public Shape retrieve() {
		return(copy(shape));
	}

	/**
		Clones a Shape.
		@param s The Shape to Clone().
		@return Cloned Shape or null if (s == null) ||
		CloneNotSupportedException.
	*/
	private Shape copy(Shape s) {
		// For those who don't know, dolly is the name of the first
		// cloned animal (sheep)
		Shape	dolly = null;

		if (s != null) {
			try {
				dolly = (Shape)s.clone();
			} catch (CloneNotSupportedException e) {
				e.printStackTrace();
			}
		}

		return(dolly);
	}
}
