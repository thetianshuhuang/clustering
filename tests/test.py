import numpy as np
from matplotlib import pyplot as plt
import time
from tqdm import tqdm

import bclust

import faulthandler
faulthandler.enable()


SCALE = 100
ITERATIONS = 5000


dataset = bclust.GaussianMixture(
    n=1000, k=3, d=2, r=0.7, alpha=40, symmetric=False, shuffle=False)

# dataset.plot_actual()
# plt.show()

dataset.plot_oracle()
plt.show()

test = bclust.GibbsMixtureModel(
    data=dataset.data,
    component_model=bclust.NormalWishart(),
    mixture_model=bclust.DPM(alpha=1),
    assignments=np.zeros(10 * SCALE).astype(np.uint16),
    thinning=10)

start = time.time()
for i in tqdm(range(ITERATIONS)):
    test.iter()
print("gibbs_iterate: {:.2f} s [{:.2f} ms/iteration]".format(
    time.time() - start,
    (time.time() - start) * 1000 / ITERATIONS))

start = time.time()
res = test.select_lstsq(burn_in=100)
res.evaluate(dataset.assignments, oracle=dataset.oracle)
print("evaluate_lstsq: {:.2f} s".format(time.time() - start))

res.trace()
plt.show()
res.matrices()
plt.show()
res.clustering()
plt.show()
