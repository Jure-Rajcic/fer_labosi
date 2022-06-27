import java.util.ArrayList;
import java.util.LinkedList;

public class PohlepniAlgoritam {

    static String pattern = "";
    static int minSize = Integer.MAX_VALUE;
    static ArrayList<Vertex> vg;

    public static int start() {
        int n = main.n;
        vg = new ArrayList<Vertex>();
        for (int i = 0; i < n; i++) vg.add(new Vertex(i + 1));



        // what if we have more than 1 edge which has minimal distance
        LinkedList<Pair<Vertex, Vertex>> list = new LinkedList<>();
        int currSize;

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) {
                Vertex vi = vg.get(i);
                Vertex vj = vg.get(j);
                currSize = Vertex.edgeSize(vi, vj);
                if (currSize <= minSize) {
                    if(currSize < minSize) list.clear();
                    list.add(new Pair<Vertex, Vertex>(vi, vj));
                    minSize = currSize;
                }
            }
        minSize = Integer.MAX_VALUE;

        for(Pair<Vertex, Vertex> p : list) {
            Vertex v1 = p.key, v2 = p.value;
            currSize = Vertex.edgeSize(v1, v2);
            String currPattern = "";
            currPattern = currPattern + Vertex.patternPohlepni(v1, v2);
            for (int i = 0; i < n - 2; i++) { // need to chose between n - 2 vertexes
                v1.setConnected(true);
                v2.setConnected(true);
                Pair<Integer, Vertex> pair1 = v1.getMin(), pair2 = v2.getMin();
                if (pair1.key < pair2.key) { // ako se nenade niti jedan vrh onda pair key = MaxInteger
                    currPattern = currPattern + Vertex.patternPohlepni(v1, pair1.value);
                    v1 = pair1.value; // updating v1;
                    currSize += pair1.key;
                } else {
                    currPattern = currPattern + Vertex.patternPohlepni(v2, pair2.value);
                    v2 = pair2.value;
                    currSize += pair2.key;
                }
            }
            currPattern = currPattern + Vertex.patternPohlepni(v1, v2);
            currSize += Vertex.edgeSize(v1, v2); // no need to set their boolean variable to true because program finishes here
            if(currSize < minSize) {
                minSize = currSize;
                pattern = currPattern;
            }
            for(int i = 0; i < n; i++) vg.get(i).setConnected(false);
        }
        main.pattern = pattern.substring(0, pattern.length() - " ---> ".length());
        return minSize;
    }
}
