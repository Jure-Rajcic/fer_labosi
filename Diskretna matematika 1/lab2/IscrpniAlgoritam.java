public class IscrpniAlgoritam {

    static String pattern = "";
    static int minSize = Integer.MAX_VALUE;
    static int n; // size of arrays

    public static int start() {
        n = main.n - 1;
        Vertex vertices[] = new Vertex[n];
        for(int i = 0; i < n; i++) vertices[i] = new Vertex(i + 2);
        Recursion(n, vertices);
        main.pattern = pattern;
        return minSize;
    }

    private static void Recursion(int n, Vertex[] elements) {
        if(n == 1) {
            formPattern(elements);
        } else {
            for(int i = 0; i < n-1; i++) {
                Recursion(n - 1, elements);
                if(n % 2 == 0) swap(elements, i, n-1);
                else swap(elements, 0, n-1);
            }
            Recursion(n - 1, elements);
        }
    }

    private static void formPattern(Vertex[] vertices) {
        Vertex start = new Vertex(1); // 1 will always be start
        int currSize = Vertex.edgeSize(start, vertices[0]);
        String currPattern = main.TEXT_RED + start.number + main.TEXT_RESET;
        currPattern = currPattern + Vertex.patternIscrpni(start, vertices[0]);

        for(int i = 0; i < n - 1; i++){
            Vertex previous = vertices[i];
            Vertex next = vertices[i+1];
            currSize += Vertex.edgeSize(previous, next);
            currPattern = currPattern + Vertex.patternIscrpni(previous, next);
        }
        currSize += Vertex.edgeSize(vertices[n-1], start);
        currPattern = currPattern + Vertex.patternIscrpni(vertices[n-1], start);

        if(currSize < minSize){
            minSize = currSize;
            pattern = currPattern;
        }
    }

    private static void swap(Vertex[] input, int a, int b) {
        Vertex tmp = input[a];
        input[a] = input[b];
        input[b] = tmp;
    }
}
