import java.io.*;
import java.time.*;
import java.util.*;

public class Main {

    static Vertex[] v;
    static int chromaticNumber;

    public static void main(String[] args) {
//        LocalDateTime program_start = LocalDateTime.now();
//        String output = "";
        Scanner s = new Scanner(System.in);
        System.out.print("Unesite ime datoteke: ");
        String filename = s.nextLine().strip();
        try (Scanner sc = new Scanner(new FileReader(filename));
//            Writer wr = new FileWriter("out.txt");
            ) {
            while(sc.hasNext()) {
                int n = sc.nextInt();
                v = new Vertex[n];

                int m = sc.nextInt();
                int[] S = new int[m];
                for (int i = 0; i < m; i++) S[i] = sc.nextInt();
                Arrays.sort(S);

                Pair<Integer, Vertex>[] pairs = new Pair[n];
                for (int i = 0; i < n; i++) v[i] = new Vertex(i, -1);
                for (int i = 0; i < n; i++) {
                    int degree = 0;
                    for (int j = 0; j < m && (i + S[j] < n); j++, degree++) {
                        v[i].neighbours.add(v[i + S[j]]);
                        v[i + S[j]].neighbours.add(v[i]);
                    }
                    pairs[i] = new Pair<>(degree, v[i]);
                }
                Arrays.sort(pairs, Comparator.comparingInt(o -> -o.degree));

                chromaticNumber = n;
                pairs[0].vertex.color = 0;
                while (backtrackingAlgorithm(pairs, 1)) {
                    chromaticNumber--;
                }
                System.out.println("Kromatski broj zadanog grafa je " + "\033[32m" + chromaticNumber + "\u001B[0m");
//                output += chromaticNumber + "\n";
            }
//            wr.write(output);
        } catch (IOException e) { e.printStackTrace(); }
//        System.out.println("Program took " + Duration.between(program_start, LocalDateTime.now()).toMillis() + " milliseconds to finish");
        s.close();
    }

    public static boolean backtrackingAlgorithm(Pair<Integer, Vertex>[] pairs, int index) {
        if (index == pairs.length) {
            int curr_chromaticNumber = 0;
            for(Vertex vertex : v) if (vertex.color > curr_chromaticNumber) curr_chromaticNumber = vertex.color;
            if(++curr_chromaticNumber < chromaticNumber) { chromaticNumber = curr_chromaticNumber; return true; }
            return false;
        }
        Vertex v1 = pairs[index++].vertex;
        for (int i = 0; i < chromaticNumber - 1; i++) {
            if (v1.colorGood(i)) {
                v1.color = i;
                backtrackingAlgorithm(pairs, index);
            }
        }
        return false;
    }
}

class Vertex {
    int number;
    int color;
    LinkedList<Vertex> neighbours = new LinkedList<>();

    public Vertex(int number, int color) {
        this.number = number;
        this.color = color;
    }

    boolean colorGood(int color){
        for(int i = 0; i < neighbours.size(); i++) if(color == neighbours.get(i).color) return false;
        return true;
    }
}

class Pair <K, V>{
    K degree;
    V vertex;

    public Pair(K deg, V number) {
        this.degree = deg;
        this.vertex = number;
    }
}

//graf.txt
//19
//9
//2 4 5 6 13 17 8 1 12
