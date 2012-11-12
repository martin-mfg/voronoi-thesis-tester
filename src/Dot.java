import java.awt.Color;
import java.awt.Graphics2D;

public class Dot extends Catchable implements Paintable {

	public static enum DotColor {
		RED, BLUE;
	}

	private double x;

	private double y;

	private DotColor color;

	public Dot(int x, int y, DotColor col) {
		super();
		this.x = x;
		this.y = y;
		this.color = col;
		catchPoints.add(new CoordinatePair(x, y));
	}

	public Dot(int x, int y) {
		this(x, y, DotColor.RED);
	}

	@Override
	public void paintObject(Graphics2D g2) {
		g2.setColor(color == DotColor.RED ? Color.RED : Color.BLUE);
		int drawX = (int) (x + .5);
		int drawY = (int) (y + .5);
		g2.drawLine(drawX - 2, drawY, drawX + 2, drawY);
		g2.drawLine(drawX, drawY - 2, drawX, drawY + 2);
		g2.setColor(Color.BLACK);
		if (isCaught()) {
			g2.drawOval(drawX - 4, drawY - 4, 8, 8);
		}
	}

	private void setPos(double x, double y) {
		this.x = x;
		this.y = y;
		catchPoints.clear();
		catchPoints.add(new CoordinatePair((int) (x + .5), (int) (y + .5)));
	}

	@Override
	public CoordinatePair getDragPosition() {
		return new CoordinatePair((int) (x + .5), (int) (y + .5));
	}

	@Override
	public void moveTo(double x, double y) {
		setPos(x, y);
	}

	@Override
	public PaintableType getType() {
		return PaintableType.Point;
	}
}