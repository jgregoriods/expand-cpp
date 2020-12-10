import numpy as np
import multiprocessing as mp                
import copy as cp
from tqdm import tqdm

from subprocess import Popen, PIPE


class Gene:
    def __init__(self, min_, max_, step):
        self.vals = list(range(min_, max_ + step, step))
        self.val = np.random.choice(self.vals)

    def mutate(self):
        self.val = np.random.choice([i for i in self.vals if i != self.val])


class Genome:
    def __init__(self, genes = None):
        self.fitness = 'OFF'

        self.genes = genes or [Gene(50, 250, 10),   # Fission
                               Gene(20, 100, 10),   # Carrying capacity
                               Gene(10, 30, 10),    # Permanence
                               Gene(0, 30, 5)]     # Leap

    def mutate(self, prob):
        if prob > np.random.uniform(0, 1):
            mpoint = np.random.choice(range(0, len(self.genes)))
            self.genes[mpoint].mutate()
            self.fitness = 'OFF'

    def get_gene_values(self):
        return [i.val for i in self.genes]

    def Print(self): 
        print('\nFitness: %s\n' % self.fitness)
        print(self.get_gene_values())
        print('')


class GA:
    def __init__(self, name, start, n_pop, n_select, n_elit, prob_cross, prob_mut):
        self.name = name
        self.start = start
        self.n_pop = n_pop
        self.n_select = n_select
        self.n_elit = n_elit
        self.prob_cross = prob_cross
        self.prob_mut = prob_mut
        self.population = [Genome() for i in range(self.n_pop)]
        self.best_scores = []
        self.mean_scores = []

    def run_model(self, genes):
        fiss = genes[0]
        k = genes[1]
        perm = genes[2]
        leap = genes[3]

        #### Modify here for the site, maxent and vegetation ####
        result = Popen(["./expand", f"--cult={self.name}", f"--date={self.start}", "--site=encontro",
                        f"--fiss={fiss}", f"--k={k}", f"--perm={perm}", f"--leap={leap}",
                        "--max=0.18", "--veg=0.5"],
                        stdout=PIPE).communicate()[0]
        #########################################################

        return float(result)

    def get_fitness(self):
        pop_subset = [p for p in self.population if isinstance(p.fitness, str)]
        pop_genes = [i.get_gene_values() for i in pop_subset]
        ############################
        pool = mp.Pool(18) # n cores
        ############################
        fitness = np.array(pool.map(self.run_model, pop_genes))
        pool.close()
        for i in range(len(pop_subset)):
            pop_subset[i].fitness = fitness[i]
        return np.array([p.fitness for p in self.population])

    def get_parents(self, fitness):
        parents = [self.population[i]
                   for i in np.argsort(-fitness)[range(self.n_select)]]
        return parents

    def do_crossover(self, parents): 
        crossovers = []
        ind1 = 0
        ind2 = 1
        while len(crossovers) < self.n_pop: 
            parent1 = parents[ind1]
            parent2 = parents[ind2]
            if self.prob_cross > np.random.uniform(0, 1): 
                pt = np.random.choice(range(1, len(parent1.genes)))
                child1 = Genome(parent1.genes[0:pt] + parent2.genes[pt:len(parent2.genes)])
                child2 = Genome(parent2.genes[0:pt] + parent1.genes[pt:len(parent1.genes)])
                crossovers.append(cp.deepcopy(child1))
                crossovers.append(cp.deepcopy(child2))
            else:
                crossovers.append(cp.deepcopy(parent1))
                crossovers.append(cp.deepcopy(parent2))
            ind1 = int(np.where(ind1 != len(parents)-1, ind1 + 1, 0))
            ind2 = int(np.where(ind2 != len(parents)-1, ind2 + 1, 0))
        return crossovers

    def do_mutation(self, crossovers):
        for i in crossovers:
            i.mutate(self.prob_mut)
        return crossovers

    def evolve(self, max_it):
        for it in tqdm(range(max_it)):
            fitness = self.get_fitness()
            self.mean_scores.append(np.mean(fitness))

            parents = self.get_parents(fitness)
            self.best_scores.append(parents[0].fitness)

            crossovers = self.do_crossover(parents)
            mutated = self.do_mutation(crossovers)
            self.population = mutated + [parents[i] for i in range(self.n_elit)]

            best = parents[0].get_gene_values() + [parents[0].fitness]
            np.savetxt('best.csv', best, delimiter=',')

            scores = np.asarray([self.mean_scores, self.best_scores]).transpose()
            np.savetxt('scores.csv', scores, delimiter=',')


def main():
    ga = GA("tupi", 4400, 500, 200, 50, 0.8, 0.2)
    ga.evolve(20)


if __name__ == "__main__":
    main()
