import FWCore.ParameterSet.Config as cms

from RecoHGCal.TICL.TICLSeedingRegions_cff import ticlSeedingGlobal, ticlSeedingGlobalHFNose
from RecoHGCal.TICL.trackstersProducer_cfi import trackstersProducer as _trackstersProducer
from RecoHGCal.TICL.filteredLayerClustersProducer_cfi import filteredLayerClustersProducer as _filteredLayerClustersProducer
from RecoHGCal.TICL.multiClustersFromTrackstersProducer_cfi import multiClustersFromTrackstersProducer as _multiClustersFromTrackstersProducer

# CLUSTER FILTERING/MASKING

filteredLayerClustersEM = _filteredLayerClustersProducer.clone(
    clusterFilter = "ClusterFilterByAlgoAndSizeAndLayerRange",
    min_cluster_size = 3, # inclusive
    max_layerId = 30, # inclusive
    algo_number = 8,
    LayerClustersInputMask = 'ticlTrackstersTrkEM',
    iteration_label = "EM"
)

# CA - PATTERN RECOGNITION

ticlTrackstersEM = _trackstersProducer.clone(
    filtered_mask = "filteredLayerClustersEM:EM",
    original_mask = 'ticlTrackstersTrkEM',
    seeding_regions = "ticlSeedingGlobal",
    filter_on_categories = [0, 1],
    pid_threshold = 0.5,
    energy_em_over_total_threshold = 0.9,
    max_longitudinal_sigmaPCA = 10,
    shower_start_max_layer = 5, #inclusive
    max_out_in_hops = 1,
    skip_layers = 2,
    max_missing_layers_in_trackster = 1,
    min_layers_per_trackster = 10,
    min_cos_theta = 0.97,  # ~14 degrees
    min_cos_pointing = 0.9, # ~25 degrees
    max_delta_time = 3.,
    itername = "EM",
    algo_verbosity = 0,
)

# MULTICLUSTERS

ticlMultiClustersFromTrackstersEM = _multiClustersFromTrackstersProducer.clone(
    Tracksters = "ticlTrackstersEM"
)

ticlEMStepTask = cms.Task(ticlSeedingGlobal
    ,filteredLayerClustersEM
    ,ticlTrackstersEM
    ,ticlMultiClustersFromTrackstersEM)

filteredLayerClustersHFNoseEM = filteredLayerClustersEM.clone(
    LayerClusters = 'hgcalLayerClustersHFNose',
    LayerClustersInputMask = 'ticlTrackstersHFNoseTrkEM',
    iteration_label = "EMn",
    min_cluster_size = 2, # inclusive
    algo_number = 9
)

ticlTrackstersHFNoseEM = ticlTrackstersEM.clone(
    detector = "HFNose",
    layer_clusters = "hgcalLayerClustersHFNose",
    layer_clusters_hfnose_tiles = "ticlLayerTileHFNose",
    original_mask = "ticlTrackstersHFNoseTrkEM",
    filtered_mask = "filteredLayerClustersHFNoseEM:EMn",
    seeding_regions = "ticlSeedingGlobalHFNose",
    time_layerclusters = "hgcalLayerClustersHFNose:timeLayerCluster",
    itername = "EMn",
    filter_on_categories = [0, 1],
    min_layers_per_trackster = 5,
    pid_threshold = 0.,
    shower_start_max_layer = 5 ### inclusive
)

ticlHFNoseEMStepTask = cms.Task(ticlSeedingGlobalHFNose
                              ,filteredLayerClustersHFNoseEM
                              ,ticlTrackstersHFNoseEM
)
