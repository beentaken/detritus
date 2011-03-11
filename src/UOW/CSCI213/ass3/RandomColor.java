// $Id: RandomColor.java 2 2007-07-19 13:00:48Z pdezwart $

import java.awt.*;

public class RandomColor {
	public static Color MakeColor() {
		return(new Color((float)Math.random(), (float)Math.random(),
			(float)Math.random()));
	}
}
