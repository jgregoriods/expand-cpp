# ExPaND C++

This is a C++ implementation of the agent-based model of South American population expansions originally written in Python (<a>https://github.com/jgregoriods/expand</a>). The model simulates demic diffusion from the growth, fission and relocation of individual villages. For details of the original implementation, see <a href=" https://doi.org/10.1371/journal.pone.0232367">Souza et al. (2020)</a>.

The process of expansion is determined by the following parameters: start date, centre of origin, carrying capacity, growth rate, fission threshold, permanence time and leap distance.

The model starts with a village at the centre of origin. At every time step (corresponding to a year), for every village in the model, the population grows according to the growth rate; if population is above carrying capacity, new cells can be added to the village's catchment. When population is above the fission threshold, the village splits in half and a new village is created, moving to the most suitable cell in the neighborhood, if any. When the neighborhood is saturated, if leapfrogging is allowed, a new search is performed at leap distance. Finally, if a village has been in the same cell longer than the maximum permanence time, a similar procedure is performed to search for a cell to move.

The 

## Usage

```
./expand --date=5000 --site=encontro --fiss=150 --k=20 --r=0.025 --perm=10 --leap=150 --suit=0.25 --show.bar --write
```

<img src="img/abmc.jpg" width="500">