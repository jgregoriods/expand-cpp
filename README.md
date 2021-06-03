# ExPaND C++

This is a C++ implementation of the agent-based model of South American population expansions originally written in Python (<a>https://github.com/jgregoriods/expand</a>). The model simulates demic diffusion from the growth, fission and relocation of individual villages.

The process of expansion is determined by the following parameters: start date, centre of origin, carrying capacity, growth rate, fission threshold, permanence time and leap distance.

The model starts with a village at the centre of origin. At every time step (corresponding to a year), for every village in the model, the population grows according to the growth rate; if population is above carrying capacity, 

## Usage

```
./expand --date=5000 --site=encontro --fiss=150 --k=20 --r=0.025 --perm=10 --leap=150 --suit=0.25 --show.bar --write
```