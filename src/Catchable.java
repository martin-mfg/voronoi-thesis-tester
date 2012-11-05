import java.util.HashSet;
import java.util.Set;

public abstract class Catchable implements Paintable {

	protected Set<CoordinatePair>	catchPoints	= new HashSet<CoordinatePair>();

	private boolean					isCaught;

	public Set<CoordinatePair> getCatchPoints() {
		return catchPoints;
	}

	public double getDistanceTo(CoordinatePair cp) {
		double retVal = Double.MAX_VALUE;
		for (CoordinatePair catchPoint : catchPoints) {
			double dist = cp.distanceTo(catchPoint);
			retVal = dist < retVal ? dist : retVal;
		}
		return retVal;
	}

	public abstract CoordinatePair getDragPosition();

	public abstract void moveTo(double x, double y);

	public boolean isCaught() {
		return isCaught;
	}

	public void setCaught(boolean isCaught) {
		this.isCaught = isCaught;
	}

}
