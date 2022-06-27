public class Vertex {
    int number;
    boolean conected;


    public Vertex(int number) {
        this.number = number;
        conected = false;
    }

    public void setConnected(boolean conected) {
        this.conected = conected;
    }

    public static int edgeSize(Vertex v1, Vertex v2){
        if(v2.number < v1.number) {
            Vertex v = v1;
            v1 = v2;
            v2 = v;
        }
        int size = main.a * v1.number + main.b * v2.number;
        return size * size + 1;
    }


    public Pair<Integer, Vertex> getMin(){
        int n = main.n, minSize = Integer.MAX_VALUE;
        Vertex v = null;

        for (int i = 0; i < n; i++){
            if(number == i + 1) continue; // same Vartex we skip
            Vertex vi = PohlepniAlgoritam.vg.get(i);

            int currSize = Vertex.edgeSize(this, vi);
            if (currSize < minSize && !vi.conected) {
                minSize = currSize;
                v = vi;
            }
        }
        return new Pair<Integer, Vertex>(minSize, v);
    }

    public static String patternPohlepni(Vertex v1, Vertex v2){
        return "(" + v1.number + ", " + v2.number + ")" + " " + main.TEXT_RED + edgeSize(v1, v2) + main.TEXT_RESET + " ---> ";
    }

    public static String patternIscrpni(Vertex v1, Vertex v2){
        return " --" + edgeSize(v1, v2) + "--> " + main.TEXT_RED + v2.number + main.TEXT_RESET;
    }

    @Override
    public String toString() {
        return "" + number;
    }
}
