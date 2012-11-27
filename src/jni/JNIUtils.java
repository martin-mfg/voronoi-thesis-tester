package jni;

public class JNIUtils {

	private static native ResponseArrangement getArrangement(double[] points);

	static {
		System.loadLibrary("cgal_wrappers");
	}

	private class ResponseArrangement {

		double[]	circles;	//
	}
}
