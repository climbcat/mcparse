### Example bulk output for mcparse V0.1.0:

Using the following, we can test the entire test library of components and instruments. (The printed errors represent not-implemented parsing features.)

For timings, scroll down to see the profiler output near the end of this file.

<pre>
./lib/getdeps.sh
./build.sh
./mcparse --version
./mcparse --comps mcstas-comps/ --instrs mcstas-comps/
</pre>

<pre>
parsing  #000: mcstas-comps/samples/Single_magnetic_crystal.comp
parsing  #001: mcstas-comps/samples/Magnon_bcc.comp
parsing  #002: mcstas-comps/samples/Single_crystal.comp
parsing  #003: mcstas-comps/samples/Res_sample.comp
parsing  #004: mcstas-comps/samples/TOFRes_sample.comp
parsing  #005: mcstas-comps/samples/SANS_spheres2.comp
parsing  #006: mcstas-comps/samples/NCrystal_sample.comp
parsing  #007: mcstas-comps/samples/Tunneling_sample.comp
parsing  #008: mcstas-comps/samples/Powder1.comp
parsing  #009: mcstas-comps/samples/PowderN.comp
parsing  #010: mcstas-comps/samples/Phonon_simple.comp
parsing  #011: mcstas-comps/samples/Sans_spheres.comp
parsing  #012: mcstas-comps/samples/SasView_model.comp
parsing  #013: mcstas-comps/samples/Incoherent.comp
parsing  #014: mcstas-comps/samples/Isotropic_Sqw.comp
parsing  #015: mcstas-comps/misc/MCPL_output_noacc.comp
parsing  #016: mcstas-comps/misc/Scatter_logger.comp
parsing  #017: mcstas-comps/misc/Circle.comp
parsing  #018: mcstas-comps/misc/Cone.comp
parsing  #019: mcstas-comps/misc/Disc.comp
parsing  #020: mcstas-comps/misc/MCPL_input.comp
parsing  #021: mcstas-comps/misc/Shape.comp
parsing  #022: mcstas-comps/misc/Annulus.comp
parsing  #023: mcstas-comps/misc/Shape_simple.comp
parsing  #024: mcstas-comps/misc/Legacy_circle.comp
parsing  #025: mcstas-comps/misc/Scatter_logger_stop.comp
parsing  #026: mcstas-comps/misc/File.comp
parsing  #027: mcstas-comps/misc/MCPL_output.comp
parsing  #028: mcstas-comps/misc/Progress_bar.comp
parsing  #029: mcstas-comps/misc/MCPL_input_once.comp
parsing  #030: mcstas-comps/misc/Scatter_log_iterator.comp

ERROR: Expected 'identifier', got '('

76,6|   int (*pseudo_neutron_state_function) (double *, struct Generalized_State_t *, struct Generalized_State_t *); 
            ^

parsing  #031: mcstas-comps/misc/Scatter_log_iterator_stop.comp
parsing  #032: mcstas-comps/sources/Adapt_check.comp
parsing  #033: mcstas-comps/sources/Source_Maxwell_3.comp
parsing  #034: mcstas-comps/sources/Source_div.comp
parsing  #035: mcstas-comps/sources/Monitor_Optimizer.comp
parsing  #036: mcstas-comps/sources/Source_div_quasi.comp
parsing  #037: mcstas-comps/sources/ESS_butterfly.comp
parsing  #038: mcstas-comps/sources/Source_simple.comp
parsing  #039: mcstas-comps/sources/Source_gen.comp
parsing  #040: mcstas-comps/sources/Source_adapt.comp
parsing  #041: mcstas-comps/sources/ESS_moderator.comp
parsing  #042: mcstas-comps/sources/Moderator.comp
parsing  #043: mcstas-comps/sources/Source_4PI.comp
parsing  #044: mcstas-comps/sources/Source_Optimizer.comp
parsing  #045: mcstas-comps/sasmodels/SasView_hollow_rectangular_prism_thin_walls.comp
parsing  #046: mcstas-comps/sasmodels/SasView_core_shell_cylinder_aniso.comp
parsing  #047: mcstas-comps/sasmodels/SasView_stacked_disks_aniso.comp
parsing  #048: mcstas-comps/sasmodels/SasView_multilayer_vesicle.comp
parsing  #049: mcstas-comps/sasmodels/SasView_adsorbed_layer.comp
parsing  #050: mcstas-comps/sasmodels/SasView_core_shell_ellipsoid_aniso.comp
parsing  #051: mcstas-comps/sasmodels/SasView_onion.comp
parsing  #052: mcstas-comps/sasmodels/SasView_fcc_paracrystal.comp
parsing  #053: mcstas-comps/sasmodels/SasView_hollow_rectangular_prism.comp
parsing  #054: mcstas-comps/sasmodels/SasView_correlation_length.comp
parsing  #055: mcstas-comps/sasmodels/SasView_core_shell_cylinder.comp
parsing  #056: mcstas-comps/sasmodels/SasView_surface_fractal.comp
parsing  #057: mcstas-comps/sasmodels/SasView_poly_gauss_coil.comp
parsing  #058: mcstas-comps/sasmodels/SasView_elliptical_cylinder.comp
parsing  #059: mcstas-comps/sasmodels/SasView_pearl_necklace.comp
parsing  #060: mcstas-comps/sasmodels/SasView_peak_lorentz.comp
parsing  #061: mcstas-comps/sasmodels/SasView_lamellar_hg.comp
parsing  #062: mcstas-comps/sasmodels/SasView_flexible_cylinder_elliptical.comp
parsing  #063: mcstas-comps/sasmodels/SasView_capped_cylinder_aniso.comp
parsing  #064: mcstas-comps/sasmodels/SasView_polymer_excl_volume.comp
parsing  #065: mcstas-comps/sasmodels/SasView_fractal.comp
parsing  #066: mcstas-comps/sasmodels/SasView_two_lorentzian.comp
parsing  #067: mcstas-comps/sasmodels/SasView_bcc_paracrystal_aniso.comp
parsing  #068: mcstas-comps/sasmodels/SasView_hollow_rectangular_prism_aniso.comp
parsing  #069: mcstas-comps/sasmodels/SasView_triaxial_ellipsoid.comp
parsing  #070: mcstas-comps/sasmodels/SasView_lamellar_hg_stack_caille.comp
parsing  #071: mcstas-comps/sasmodels/SasView_parallelepiped_aniso.comp
parsing  #072: mcstas-comps/sasmodels/SasView_pringle.comp
parsing  #073: mcstas-comps/sasmodels/SasView_linear_pearls.comp
parsing  #074: mcstas-comps/sasmodels/SasView_flexible_cylinder.comp
parsing  #075: mcstas-comps/sasmodels/SasView_rpa.comp
parsing  #076: mcstas-comps/sasmodels/SasView_cylinder.comp
parsing  #077: mcstas-comps/sasmodels/SasView_rectangular_prism_aniso.comp
parsing  #078: mcstas-comps/sasmodels/SasView_core_multi_shell.comp
parsing  #079: mcstas-comps/sasmodels/SasView_mass_fractal.comp
parsing  #080: mcstas-comps/sasmodels/SasView_barbell.comp
parsing  #081: mcstas-comps/sasmodels/SasView_core_shell_bicelle_elliptical_aniso.comp
parsing  #082: mcstas-comps/sasmodels/SasView_core_shell_ellipsoid.comp
parsing  #083: mcstas-comps/sasmodels/SasView_core_shell_bicelle_elliptical_belt_rough.comp
parsing  #084: mcstas-comps/sasmodels/SasView_fcc_paracrystal_aniso.comp
parsing  #085: mcstas-comps/sasmodels/SasView_lorentz.comp
parsing  #086: mcstas-comps/sasmodels/SasView_binary_hard_sphere.comp
parsing  #087: mcstas-comps/sasmodels/SasView_hollow_cylinder_aniso.comp
parsing  #088: mcstas-comps/sasmodels/SasView_polymer_micelle.comp
parsing  #089: mcstas-comps/sasmodels/SasView_fuzzy_sphere.comp
parsing  #090: mcstas-comps/sasmodels/SasView_dab.comp
parsing  #091: mcstas-comps/sasmodels/SasView_barbell_aniso.comp
parsing  #092: mcstas-comps/sasmodels/SasView_ellipsoid_aniso.comp
parsing  #093: mcstas-comps/sasmodels/SasView_core_shell_bicelle_aniso.comp
parsing  #094: mcstas-comps/sasmodels/SasView_star_polymer.comp
parsing  #095: mcstas-comps/sasmodels/SasView_guinier_porod.comp
parsing  #096: mcstas-comps/sasmodels/SasView_cylinder_aniso.comp
parsing  #097: mcstas-comps/sasmodels/SasView_teubner_strey.comp
parsing  #098: mcstas-comps/sasmodels/SasView_gaussian_peak.comp
parsing  #099: mcstas-comps/sasmodels/SasView_core_shell_parallelepiped_aniso.comp
parsing  #100: mcstas-comps/sasmodels/SasView_elliptical_cylinder_aniso.comp
parsing  #101: mcstas-comps/sasmodels/SasView_triaxial_ellipsoid_aniso.comp
parsing  #102: mcstas-comps/sasmodels/SasView_core_shell_parallelepiped.comp
parsing  #103: mcstas-comps/sasmodels/SasView_core_shell_bicelle_elliptical_belt_rough_aniso.comp
parsing  #104: mcstas-comps/sasmodels/SasView_fractal_core_shell.comp
parsing  #105: mcstas-comps/sasmodels/SasView_squarewell.comp
parsing  #106: mcstas-comps/sasmodels/SasView_power_law.comp
parsing  #107: mcstas-comps/sasmodels/SasView_bcc_paracrystal.comp
parsing  #108: mcstas-comps/sasmodels/SasView_broad_peak.comp
parsing  #109: mcstas-comps/sasmodels/SasView_two_power_law.comp
parsing  #110: mcstas-comps/sasmodels/SasView_mass_surface_fractal.comp
parsing  #111: mcstas-comps/sasmodels/SasView_vesicle.comp
parsing  #112: mcstas-comps/sasmodels/SasView_mono_gauss_coil.comp
parsing  #113: mcstas-comps/sasmodels/SasView_line.comp
parsing  #114: mcstas-comps/sasmodels/SasView_guinier.comp
parsing  #115: mcstas-comps/sasmodels/SasView_hollow_cylinder.comp
parsing  #116: mcstas-comps/sasmodels/SasView_superball_aniso.comp
parsing  #117: mcstas-comps/sasmodels/SasView_raspberry.comp
parsing  #118: mcstas-comps/sasmodels/SasView_porod.comp
parsing  #119: mcstas-comps/sasmodels/SasView_rectangular_prism.comp
parsing  #120: mcstas-comps/sasmodels/SasView_sphere.comp
parsing  #121: mcstas-comps/sasmodels/SasView_lamellar_stack_paracrystal.comp
parsing  #122: mcstas-comps/sasmodels/SasView_hayter_msa.comp
parsing  #123: mcstas-comps/sasmodels/SasView_core_shell_sphere.comp
parsing  #124: mcstas-comps/sasmodels/SasView_hardsphere.comp
parsing  #125: mcstas-comps/sasmodels/SasView_sc_paracrystal.comp
parsing  #126: mcstas-comps/sasmodels/SasView_capped_cylinder.comp
parsing  #127: mcstas-comps/sasmodels/SasView_stickyhardsphere.comp
parsing  #128: mcstas-comps/sasmodels/SasView_core_shell_bicelle.comp
parsing  #129: mcstas-comps/sasmodels/SasView_gel_fit.comp
parsing  #130: mcstas-comps/sasmodels/SasView_spinodal.comp
parsing  #131: mcstas-comps/sasmodels/SasView_ellipsoid.comp
parsing  #132: mcstas-comps/sasmodels/SasView_superball.comp
parsing  #133: mcstas-comps/sasmodels/SasView_stacked_disks.comp
parsing  #134: mcstas-comps/sasmodels/SasView_parallelepiped.comp
parsing  #135: mcstas-comps/sasmodels/SasView_core_shell_bicelle_elliptical.comp
parsing  #136: mcstas-comps/sasmodels/SasView_lamellar_stack_caille.comp
parsing  #137: mcstas-comps/sasmodels/SasView_gauss_lorentz_gel.comp
parsing  #138: mcstas-comps/sasmodels/SasView_sc_paracrystal_aniso.comp
parsing  #139: mcstas-comps/monitors/Flex_monitor_3D.comp
parsing  #140: mcstas-comps/monitors/TOF_PSD_monitor_rad.comp
parsing  #141: mcstas-comps/monitors/PSD_monitor_4PI.comp
parsing  #142: mcstas-comps/monitors/PSD_monitor_4PI_spin.comp
parsing  #143: mcstas-comps/monitors/Monitor_nD.comp
parsing  #144: mcstas-comps/monitors/TOF2Q_cylPSD_monitor.comp
parsing  #145: mcstas-comps/monitors/Sqq_w_monitor.comp
parsing  #146: mcstas-comps/monitors/TOFLambda_monitor.comp
parsing  #147: mcstas-comps/monitors/Flex_monitor_2D.comp
parsing  #148: mcstas-comps/monitors/Monitor.comp
parsing  #149: mcstas-comps/monitors/E_monitor.comp
parsing  #150: mcstas-comps/monitors/Flex_monitor_1D.comp
parsing  #151: mcstas-comps/monitors/TOFRes_monitor.comp
parsing  #152: mcstas-comps/monitors/PolLambda_monitor.comp
parsing  #153: mcstas-comps/monitors/Sqw_monitor.comp
parsing  #154: mcstas-comps/monitors/PSD_monitor_psf_eff.comp
parsing  #155: mcstas-comps/monitors/L_monitor.comp
parsing  #156: mcstas-comps/monitors/DivLambda_monitor.comp
parsing  #157: mcstas-comps/monitors/TOFlog_monitor.comp
parsing  #158: mcstas-comps/monitors/PSDcyl_monitor.comp
parsing  #159: mcstas-comps/monitors/Cyl_monitor_TOF.comp
parsing  #160: mcstas-comps/monitors/PSDlin_diff_monitor.comp
parsing  #161: mcstas-comps/monitors/Pol_monitor.comp
parsing  #162: mcstas-comps/monitors/Event_monitor_simple.comp
parsing  #163: mcstas-comps/monitors/Res_monitor.comp
parsing  #164: mcstas-comps/monitors/Monitor_nD_noacc.comp
parsing  #165: mcstas-comps/monitors/Div1D_monitor.comp
parsing  #166: mcstas-comps/monitors/PSD_monitor.comp
parsing  #167: mcstas-comps/monitors/PSDlin_monitor.comp
parsing  #168: mcstas-comps/monitors/MeanPolLambda_monitor.comp
parsing  #169: mcstas-comps/monitors/TOF_monitor.comp
parsing  #170: mcstas-comps/monitors/Divergence_monitor.comp
parsing  #171: mcstas-comps/monitors/PreMonitor_nD.comp
parsing  #172: mcstas-comps/monitors/Cyl_monitor.comp
parsing  #173: mcstas-comps/monitors/Cyl_monitor_PSD.comp
parsing  #174: mcstas-comps/monitors/Monitor_4PI.comp
parsing  #175: mcstas-comps/monitors/Brilliance_monitor.comp
parsing  #176: mcstas-comps/monitors/TOF_cylPSD_monitor.comp
parsing  #177: mcstas-comps/monitors/PSD_monitor_TOF.comp
parsing  #178: mcstas-comps/monitors/DivPos_monitor.comp
parsing  #179: mcstas-comps/monitors/TOF2E_monitor.comp
parsing  #180: mcstas-comps/monitors/PSD_monitor_psf.comp
parsing  #181: mcstas-comps/monitors/EPSD_monitor.comp
parsing  #182: mcstas-comps/monitors/PSD_TOF_monitor.comp
parsing  #183: mcstas-comps/union/Template_process.comp
parsing  #184: mcstas-comps/union/Union_stop.comp
parsing  #185: mcstas-comps/union/Union_logger_3D_space.comp
parsing  #186: mcstas-comps/union/Union_abs_logger_1D_space.comp
parsing  #187: mcstas-comps/union/IncoherentPhonon_process.comp
parsing  #188: mcstas-comps/union/Union_conditional_standard.comp
parsing  #189: mcstas-comps/union/Union_conditional_PSD.comp
parsing  #190: mcstas-comps/union/NCrystal_process.comp
parsing  #191: mcstas-comps/union/Union_master.comp

ERROR: Expected 'identifier', got '*'

116,24|   struct Volume_struct **Volumes;
                                ^

parsing  #192: mcstas-comps/union/Union_make_material.comp
parsing  #193: mcstas-comps/union/Union_mesh.comp
parsing  #194: mcstas-comps/union/Powder_process.comp
parsing  #195: mcstas-comps/union/Union_box.comp
parsing  #196: mcstas-comps/union/Union_init.comp
parsing  #197: mcstas-comps/union/Union_abs_logger_1D_space_tof.comp
parsing  #198: mcstas-comps/union/Union_logger_2D_kf_time.comp
parsing  #199: mcstas-comps/union/Union_logger_2D_kf.comp
parsing  #200: mcstas-comps/union/AF_HB_1D_process.comp
parsing  #201: mcstas-comps/union/Union_logger_2DQ.comp
parsing  #202: mcstas-comps/union/Union_cylinder.comp
parsing  #203: mcstas-comps/union/Incoherent_process.comp
parsing  #204: mcstas-comps/union/Union_abs_logger_nD.comp
parsing  #205: mcstas-comps/union/Union_sphere.comp
parsing  #206: mcstas-comps/union/Single_crystal_process.comp
parsing  #207: mcstas-comps/union/Union_logger_2D_space.comp
parsing  #208: mcstas-comps/union/Union_abs_logger_2D_space.comp
parsing  #209: mcstas-comps/union/Union_cone.comp
parsing  #210: mcstas-comps/union/Union_abs_logger_1D_time.comp
parsing  #211: mcstas-comps/union/Union_abs_logger_event.comp
parsing  #212: mcstas-comps/union/Union_master_GPU.comp

ERROR: Expected 'identifier', got '*'

115,24|   struct Volume_struct **Volumes;
                                ^

parsing  #213: mcstas-comps/union/Texture_process.comp
parsing  #214: mcstas-comps/union/Union_logger_1D.comp
parsing  #215: mcstas-comps/union/PhononSimple_process.comp
parsing  #216: mcstas-comps/union/Union_abs_logger_1D_space_tof_to_lambda.comp
parsing  #217: mcstas-comps/union/Non_process.comp
parsing  #218: mcstas-comps/union/Union_abs_logger_1D_space_event.comp
parsing  #219: mcstas-comps/union/Union_logger_2D_space_time.comp
parsing  #220: mcstas-comps/optics/Beamstop.comp
parsing  #221: mcstas-comps/optics/Absorber.comp
parsing  #222: mcstas-comps/optics/Derotator.comp
parsing  #223: mcstas-comps/optics/Collimator_linear.comp
parsing  #224: mcstas-comps/optics/Pol_bender.comp
parsing  #225: mcstas-comps/optics/He3_cell.comp
parsing  #226: mcstas-comps/optics/Pol_tabled_field.comp
parsing  #227: mcstas-comps/optics/Monochromator_flat.comp
parsing  #228: mcstas-comps/optics/Pol_Bfield.comp
parsing  #229: mcstas-comps/optics/Monochromator_curved.comp
parsing  #230: mcstas-comps/optics/V_selector.comp
parsing  #231: mcstas-comps/optics/Vitess_ChopperFermi.comp
parsing  #232: mcstas-comps/optics/DiskChopper.comp
parsing  #233: mcstas-comps/optics/Pol_FieldBox.comp
parsing  #234: mcstas-comps/optics/Guide_simple.comp
parsing  #235: mcstas-comps/optics/Refractor.comp
parsing  #236: mcstas-comps/optics/PolAnalyser_ideal.comp
parsing  #237: mcstas-comps/optics/Pol_mirror.comp
parsing  #238: mcstas-comps/optics/Elliptic_guide_gravity.comp
parsing  #239: mcstas-comps/optics/Rotator.comp
parsing  #240: mcstas-comps/optics/Bender.comp
parsing  #241: mcstas-comps/optics/FZP_simple.comp
parsing  #242: mcstas-comps/optics/Guide_gravity.comp
parsing  #243: mcstas-comps/optics/Arm.comp
parsing  #244: mcstas-comps/optics/Filter_gen.comp
parsing  #245: mcstas-comps/optics/Guide_anyshape.comp
parsing  #246: mcstas-comps/optics/Pol_constBfield.comp
parsing  #247: mcstas-comps/optics/Guide.comp
parsing  #248: mcstas-comps/optics/Pol_SF_ideal.comp
parsing  #249: mcstas-comps/optics/Selector.comp
parsing  #250: mcstas-comps/optics/Collimator_radial.comp
parsing  #251: mcstas-comps/optics/Mask.comp
parsing  #252: mcstas-comps/optics/Guide_channeled.comp
parsing  #253: mcstas-comps/optics/Mirror.comp
parsing  #254: mcstas-comps/optics/Pol_Bfield_stop.comp
parsing  #255: mcstas-comps/optics/FermiChopper.comp
parsing  #256: mcstas-comps/optics/Guide_wavy.comp
parsing  #257: mcstas-comps/optics/Pol_guide_vmirror.comp
parsing  #258: mcstas-comps/optics/Guide_tapering.comp
parsing  #259: mcstas-comps/optics/Set_pol.comp
parsing  #260: mcstas-comps/optics/Monochromator_pol.comp
parsing  #261: mcstas-comps/optics/Slit.comp
parsing  #262: mcstas-comps/optics/Pol_guide_mirror.comp
parsing  #263: mcstas-comps/contrib/SANSCylinders.comp
parsing  #264: mcstas-comps/contrib/Vertical_T0a.comp
parsing  #265: mcstas-comps/contrib/Filter_graphite.comp
parsing  #266: mcstas-comps/contrib/NPI_tof_dhkl_detector.comp
parsing  #267: mcstas-comps/contrib/SANSNanodiscsFast.comp
parsing  #268: mcstas-comps/contrib/TOFSANSdet.comp
parsing  #269: mcstas-comps/contrib/ISIS_moderator.comp
parsing  #270: mcstas-comps/contrib/Mirror_Parabolic.comp
parsing  #271: mcstas-comps/contrib/PSD_spinDmon.comp
parsing  #272: mcstas-comps/contrib/Conics_PP.comp
parsing  #273: mcstas-comps/contrib/Al_window.comp
parsing  #274: mcstas-comps/contrib/Vertical_Bender.comp
parsing  #275: mcstas-comps/contrib/SNS_source_analytic.comp
parsing  #276: mcstas-comps/contrib/SANS_AnySamp.comp
parsing  #277: mcstas-comps/contrib/SANSPDB.comp

ERROR: Expected ';' or '(', got *

412,16| 	double complex **Matrix;
                        ^

parsing  #278: mcstas-comps/contrib/PSD_Pol_monitor.comp
parsing  #279: mcstas-comps/contrib/Spin_random.comp
parsing  #280: mcstas-comps/contrib/Spherical_Backscattering_Analyser.comp
parsing  #281: mcstas-comps/contrib/Lens_simple.comp
parsing  #282: mcstas-comps/contrib/Monochromator_2foc.comp
parsing  #283: mcstas-comps/contrib/Guide_four_side.comp
parsing  #284: mcstas-comps/contrib/MultiDiskChopper.comp
parsing  #285: mcstas-comps/contrib/Source_pulsed.comp
parsing  #286: mcstas-comps/contrib/SANS_Liposomes_Abs.comp
parsing  #287: mcstas-comps/contrib/Exact_radial_coll.comp
parsing  #288: mcstas-comps/contrib/Pol_bender_tapering.comp
parsing  #289: mcstas-comps/contrib/SANSNanodiscsWithTagsFast.comp
parsing  #290: mcstas-comps/contrib/SANSCurve.comp
parsing  #291: mcstas-comps/contrib/Guide_curved.comp
parsing  #292: mcstas-comps/contrib/Source_gen4.comp
parsing  #293: mcstas-comps/contrib/Conics_EH.comp
parsing  #294: mcstas-comps/contrib/ViewModISIS.comp
parsing  #295: mcstas-comps/contrib/SNS_source.comp

ERROR: Expected 'identifier', got '*'

502,10|   double **Ptmat;
                  ^

parsing  #296: mcstas-comps/contrib/Source_multi_surfaces.comp
parsing  #297: mcstas-comps/contrib/Mirror_Elliptic_Bispectral.comp
parsing  #298: mcstas-comps/contrib/TOF2Q_cyl_monitor.comp
parsing  #299: mcstas-comps/contrib/SANSLiposomes.comp
parsing  #300: mcstas-comps/contrib/StatisticalChopper.comp
parsing  #301: mcstas-comps/contrib/SANS_benchmark2.comp
parsing  #302: mcstas-comps/contrib/E_4PI.comp
parsing  #303: mcstas-comps/contrib/multi_pipe.comp
parsing  #304: mcstas-comps/contrib/Foil_flipper_magnet.comp
parsing  #305: mcstas-comps/contrib/Radial_div.comp
parsing  #306: mcstas-comps/contrib/StatisticalChopper_Monitor.comp

ERROR: Expected '%{', got 'COPY'

48,8| DECLARE COPY Monitor_nD
              ^

parsing  #307: mcstas-comps/contrib/Collimator_ROC.comp
parsing  #308: mcstas-comps/contrib/Source_custom.comp
parsing  #309: mcstas-comps/contrib/Single_crystal_inelastic.comp
parsing  #310: mcstas-comps/contrib/Pol_triafield.comp
parsing  #311: mcstas-comps/contrib/SANSQMonitor.comp
parsing  #312: mcstas-comps/contrib/Fermi_chop2a.comp
parsing  #313: mcstas-comps/contrib/Sans_liposomes_new.comp
parsing  #314: mcstas-comps/contrib/Pol_pi_2_rotator.comp
parsing  #315: mcstas-comps/contrib/SupermirrorFlat.comp
parsing  #316: mcstas-comps/contrib/SANSShells.comp
parsing  #317: mcstas-comps/contrib/CavitiesOut.comp
parsing  #318: mcstas-comps/contrib/SANSPDBFast.comp
parsing  #319: mcstas-comps/contrib/Transmission_polarisatorABSnT.comp
parsing  #320: mcstas-comps/contrib/Guide_anyshape_r.comp
parsing  #321: mcstas-comps/contrib/Guide_multichannel.comp
parsing  #322: mcstas-comps/contrib/Guide_gravity_psd.comp
parsing  #323: mcstas-comps/contrib/CavitiesIn.comp
parsing  #324: mcstas-comps/contrib/SANS_DebyeS.comp
parsing  #325: mcstas-comps/contrib/Mirror_Curved_Bispectral.comp
parsing  #326: mcstas-comps/contrib/SANSNanodiscs.comp
parsing  #327: mcstas-comps/contrib/Guide_honeycomb.comp
parsing  #328: mcstas-comps/contrib/Conics_PH.comp
parsing  #329: mcstas-comps/contrib/SANS_Guinier.comp
parsing  #330: mcstas-comps/contrib/Sapphire_Filter.comp
parsing  #331: mcstas-comps/contrib/SANSNanodiscsWithTags.comp
parsing  #332: mcstas-comps/contrib/FlatEllipse_finite_mirror.comp
parsing  #333: mcstas-comps/contrib/Lens.comp
parsing  #334: mcstas-comps/contrib/PSD_spinUmon.comp
parsing  #335: mcstas-comps/contrib/Mirror_Elliptic.comp
parsing  #336: mcstas-comps/contrib/PSD_monitor_rad.comp
parsing  #337: mcstas-comps/contrib/SANSSpheresPolydisperse.comp
parsing  #338: mcstas-comps/contrib/PSD_Detector.comp
parsing  #339: mcstas-comps/contrib/SANSSpheres.comp
parsing  #340: mcstas-comps/contrib/Guide_m.comp
parsing  #341: mcstas-comps/contrib/Multilayer_Sample.comp
parsing  #342: mcstas-comps/contrib/GISANS_sample.comp
parsing  #343: mcstas-comps/contrib/FermiChopper_ILL.comp
parsing  #344: mcstas-comps/contrib/SANSEllipticCylinders.comp
parsing  #345: mcstas-comps/contrib/SiC.comp
parsing  #346: mcstas-comps/contrib/NPI_tof_theta_monitor.comp
parsing  #347: mcstas-comps/contrib/Transmission_V_polarisator.comp
parsing  #348: mcstas-comps/contrib/Conics_HE.comp
parsing  #349: mcstas-comps/contrib/Spot_sample.comp
parsing  #350: mcstas-comps/contrib/PerfectCrystal.comp
parsing  #351: mcstas-comps/contrib/Monochromator_bent.comp
parsing  #000: mcstas-comps/examples/HighNESS/WOFSANS/WOFSANS.instr
parsing  #001: mcstas-comps/examples/ILL/ILL_H5_new/ILL_H5_new.instr
parsing  #002: mcstas-comps/examples/ILL/ILL_H22_D1A/ILL_H22_D1A.instr
parsing  #003: mcstas-comps/examples/ILL/ILL_IN6/ILL_IN6.instr
parsing  #004: mcstas-comps/examples/ILL/ILL_H22/ILL_H22.instr
parsing  #005: mcstas-comps/examples/ILL/ILL_Lagrange/ILL_Lagrange.instr
parsing  #006: mcstas-comps/examples/ILL/ILL_H142_IN12/ILL_H142_IN12.instr
parsing  #007: mcstas-comps/examples/ILL/ILL_H8_IN1/ILL_H8_IN1.instr
parsing  #008: mcstas-comps/examples/ILL/ILL_H16/ILL_H16.instr
parsing  #009: mcstas-comps/examples/ILL/ILL_H25/ILL_H25.instr
parsing  #010: mcstas-comps/examples/ILL/ILL_H16_IN5/ILL_H16_IN5.instr
parsing  #011: mcstas-comps/examples/ILL/ILL_IN4/ILL_IN4.instr
parsing  #012: mcstas-comps/examples/ILL/ILL_H142/ILL_H142.instr
parsing  #013: mcstas-comps/examples/ILL/ILL_H143_LADI/ILL_H143_LADI.instr
parsing  #014: mcstas-comps/examples/ILL/ILL_H53_D16/ILL_H53_D16.instr
parsing  #015: mcstas-comps/examples/ILL/ILL_H53_IN14/ILL_H53_IN14.instr
parsing  #016: mcstas-comps/examples/ILL/ILL_H22_VIVALDI/ILL_H22_VIVALDI.instr
parsing  #017: mcstas-comps/examples/ILL/ILL_H15/ILL_H15.instr
parsing  #018: mcstas-comps/examples/ILL/ILL_H24/ILL_H24.instr
parsing  #019: mcstas-comps/examples/ILL/ILL_H15_IN6/ILL_H15_IN6.instr
parsing  #020: mcstas-comps/examples/ILL/ILL_H5/ILL_H5.instr
parsing  #021: mcstas-comps/examples/ILL/ILL_H10_IN8/ILL_H10_IN8.instr
parsing  #022: mcstas-comps/examples/ILL/ILL_H22_D1B_noenv/ILL_H22_D1B_noenv.instr
parsing  #023: mcstas-comps/examples/ILL/ILL_H25_IN22/ILL_H25_IN22.instr
parsing  #024: mcstas-comps/examples/ILL/ILL_D4/ILL_D4.instr
parsing  #025: mcstas-comps/examples/ILL/ILL_SALSA/ILL_SALSA.instr
parsing  #026: mcstas-comps/examples/ILL/ILL_IN13/ILL_IN13.instr
parsing  #027: mcstas-comps/examples/ILL/ILL_H53/ILL_H53.instr
parsing  #028: mcstas-comps/examples/ILL/ILL_D2B/ILL_D2B.instr
parsing  #029: mcstas-comps/examples/ILL/ILL_H113/ILL_H113.instr
parsing  #030: mcstas-comps/examples/ILL/ILL_H142_D33/ILL_H142_D33.instr
parsing  #031: mcstas-comps/examples/ILL/ILL_H512_D22/ILL_H512_D22.instr
parsing  #032: mcstas-comps/examples/ILL/ILL_IN5/ILL_IN5.instr
parsing  #033: mcstas-comps/examples/ILL/ILL_BRISP/ILL_BRISP.instr
parsing  #034: mcstas-comps/examples/ILL/ILL_H22_D1A_noenv/ILL_H22_D1A_noenv.instr
parsing  #035: mcstas-comps/examples/ILL/ILL_IN5_Spots/ILL_IN5_Spots.instr
parsing  #036: mcstas-comps/examples/ILL/ILL_D2B_noenv/ILL_D2B_noenv.instr
parsing  #037: mcstas-comps/examples/ILL/ILL_H15_D11/ILL_H15_D11.instr
parsing  #038: mcstas-comps/examples/ILL/ILL_H13_IN20/ILL_H13_IN20.instr
parsing  #039: mcstas-comps/examples/ILL/ILL_H22_D1B/ILL_H22_D1B.instr
parsing  #040: mcstas-comps/examples/HZB/HZB_NEAT/HZB_NEAT.instr
parsing  #041: mcstas-comps/examples/HZB/HZB_FLEX/HZB_FLEX.instr
parsing  #042: mcstas-comps/examples/LLB/LLB_6T2/LLB_6T2.instr
parsing  #043: mcstas-comps/examples/Necsa/SAFARI_MPISI/SAFARI_MPISI.instr
parsing  #044: mcstas-comps/examples/Necsa/SAFARI_PITSI/SAFARI_PITSI.instr
parsing  #045: mcstas-comps/examples/Union_demos/Time_of_flight/Time_of_flight.instr
parsing  #046: mcstas-comps/examples/Union_demos/Manual_example/Manual_example.instr
parsing  #047: mcstas-comps/examples/Union_demos/Laue_camera/Laue_camera.instr
parsing  #048: mcstas-comps/examples/Union_demos/Sample_picture_replica/Sample_picture_replica.instr
parsing  #049: mcstas-comps/examples/Union_demos/Tagging_demo/Tagging_demo.instr
parsing  #050: mcstas-comps/examples/Union_demos/External_component/External_component.instr
parsing  #051: mcstas-comps/examples/Union_demos/Demonstration/Demonstration.instr
parsing  #052: mcstas-comps/examples/Union_sample_environments/cryostat_example/cryostat_example.instr
parsing  #053: mcstas-comps/examples/Union_sample_environments/SE_usage_example/SE_usage_example.instr
parsing  #054: mcstas-comps/examples/Templates/Demo_shape_primitives/Demo_shape_primitives.instr
parsing  #055: mcstas-comps/examples/Templates/templateNMX/templateNMX.instr
parsing  #056: mcstas-comps/examples/Templates/Tomography/Tomography.instr
parsing  #057: mcstas-comps/examples/Templates/template_simple/template_simple.instr
parsing  #058: mcstas-comps/examples/Templates/Demo_shape_primitives_simple/Demo_shape_primitives_simple.instr
parsing  #059: mcstas-comps/examples/Templates/template/template.instr
parsing  #060: mcstas-comps/examples/Templates/Test_SasView_guinier/Test_SasView_guinier.instr
parsing  #061: mcstas-comps/examples/Templates/templateSasView/templateSasView.instr
parsing  #062: mcstas-comps/examples/Templates/templateTOF/templateTOF.instr
parsing  #063: mcstas-comps/examples/Templates/BTsimple/BTsimple.instr
parsing  #064: mcstas-comps/examples/Templates/templateSANS_MCPL/templateSANS_MCPL.instr
parsing  #065: mcstas-comps/examples/Templates/templateDIFF/templateDIFF.instr
parsing  #066: mcstas-comps/examples/Templates/TOF_Reflectometer/TOF_Reflectometer.instr
parsing  #067: mcstas-comps/examples/Templates/Test_SasView_bcc_paracrystal_aniso/Test_SasView_bcc_paracrystal_aniso.instr
parsing  #068: mcstas-comps/examples/Templates/templateNMX_TOF/templateNMX_TOF.instr
parsing  #069: mcstas-comps/examples/Templates/templateTAS/templateTAS.instr
parsing  #070: mcstas-comps/examples/Templates/mini/mini.instr
parsing  #071: mcstas-comps/examples/Templates/templateSANS2/templateSANS2.instr
parsing  #072: mcstas-comps/examples/Templates/templateLaue/templateLaue.instr
parsing  #073: mcstas-comps/examples/Templates/templateSANS/templateSANS.instr
parsing  #074: mcstas-comps/examples/NCrystal/Union_NCrystal_example/Union_NCrystal_example.instr
parsing  #075: mcstas-comps/examples/NCrystal/Union_NCrystal_mix_example/Union_NCrystal_mix_example.instr
parsing  #076: mcstas-comps/examples/NCrystal/NCrystal_example/NCrystal_example.instr
parsing  #077: mcstas-comps/examples/BNL/BNL_H8_simple/BNL_H8_simple.instr
parsing  #078: mcstas-comps/examples/BNL/BNL_H8/BNL_H8.instr
parsing  #079: mcstas-comps/examples/BNL/h8_test_legacy/h8_test_legacy.instr
parsing  #080: mcstas-comps/examples/ESS/ESS_butterfly_tfocus_NOFOCUS_test/ESS_butterfly_tfocus_NOFOCUS_test.instr
parsing  #081: mcstas-comps/examples/ESS/ESS_Testbeamline_HZB_V20/ESS_Testbeamline_HZB_V20.instr
parsing  #082: mcstas-comps/examples/ESS/ESS_butterfly_MCPL_test/ESS_butterfly_MCPL_test.instr
parsing  #083: mcstas-comps/examples/ESS/ESS_butterfly_test/ESS_butterfly_test.instr
parsing  #084: mcstas-comps/examples/ESS/ESS_butterfly_Guide_curved_test/ESS_butterfly_Guide_curved_test.instr
parsing  #085: mcstas-comps/examples/ESS/ESS_mcpl2hist/ESS_mcpl2hist.instr
parsing  #086: mcstas-comps/examples/ESS/ESS_IN5_reprate/ESS_IN5_reprate.instr
parsing  #087: mcstas-comps/examples/ESS/ESS_butterfly_tfocus_test/ESS_butterfly_tfocus_test.instr
parsing  #088: mcstas-comps/examples/ESS/ESS_BEER_MCPL/ESS_BEER_MCPL.instr
parsing  #089: mcstas-comps/examples/TRIGA/RTP_NeutronRadiography/RTP_NeutronRadiography.instr
parsing  #090: mcstas-comps/examples/TRIGA/RTP_SANS/RTP_SANS.instr
parsing  #091: mcstas-comps/examples/TRIGA/RTP_Laue/RTP_Laue.instr
parsing  #092: mcstas-comps/examples/TRIGA/RTP_DIF/RTP_DIF.instr
parsing  #093: mcstas-comps/examples/TUDelft/SESANS_Delft/SESANS_Delft.instr
parsing  #094: mcstas-comps/examples/TUDelft/SEMSANS_Delft/SEMSANS_Delft.instr
parsing  #095: mcstas-comps/examples/TUDelft/SEMSANS_instrument/SEMSANS_instrument.instr
parsing  #096: mcstas-comps/examples/Mantid/ISIS_TOSCA_preupgrade_Mantid/ISIS_TOSCA_preupgrade_Mantid.instr
parsing  #097: mcstas-comps/examples/Mantid/ILL_H16_Mantid/ILL_H16_Mantid.instr
parsing  #098: mcstas-comps/examples/Mantid/SNS_ARCS_Mantid/SNS_ARCS_Mantid.instr
parsing  #099: mcstas-comps/examples/Mantid/ISIS_SANS2d_Mantid/ISIS_SANS2d_Mantid.instr
parsing  #100: mcstas-comps/examples/Mantid/templateVanadiumMultipleScat_Mantid/templateVanadiumMultipleScat_Mantid.instr
parsing  #101: mcstas-comps/examples/Mantid/ILL_IN5_Mantid/ILL_IN5_Mantid.instr
parsing  #102: mcstas-comps/examples/Mantid/ILL_H16_IN5_Mantid/ILL_H16_IN5_Mantid.instr
parsing  #103: mcstas-comps/examples/Mantid/templateSANS_Mantid/templateSANS_Mantid.instr
parsing  #104: mcstas-comps/examples/Mantid/Tools_ONION/Tools_ONION.instr
parsing  #105: mcstas-comps/examples/Mantid/templateSANS2_Mantid/templateSANS2_Mantid.instr
parsing  #106: mcstas-comps/examples/Mantid/templateSasView_Mantid/templateSasView_Mantid.instr
parsing  #107: mcstas-comps/examples/ISIS/ISIS_IMAT/ISIS_IMAT.instr
parsing  #108: mcstas-comps/examples/ISIS/ISIS_TS1_Brilliance/ISIS_TS1_Brilliance.instr
parsing  #109: mcstas-comps/examples/ISIS/ISIS_TOSCA_preupgrade/ISIS_TOSCA_preupgrade.instr
parsing  #110: mcstas-comps/examples/ISIS/ISIS_HET/ISIS_HET.instr
parsing  #111: mcstas-comps/examples/ISIS/ISIS_Prisma2/ISIS_Prisma2.instr
parsing  #112: mcstas-comps/examples/ISIS/ISIS_GEM/ISIS_GEM.instr
parsing  #113: mcstas-comps/examples/ISIS/ISIS_CRISP/ISIS_CRISP.instr
parsing  #114: mcstas-comps/examples/ISIS/ISIS_TS2_Brilliance/ISIS_TS2_Brilliance.instr
parsing  #115: mcstas-comps/examples/ISIS/ISIS_OSIRIS/ISIS_OSIRIS.instr
parsing  #116: mcstas-comps/examples/ISIS/ISIS_MERLIN/ISIS_MERLIN.instr
parsing  #117: mcstas-comps/examples/ISIS/ViewModISIStest/ViewModISIStest.instr
parsing  #118: mcstas-comps/examples/ISIS/ISIS_SANS2d/ISIS_SANS2d.instr
parsing  #119: mcstas-comps/examples/ISIS/ISIS_test/ISIS_test.instr
parsing  #120: mcstas-comps/examples/FZ_Juelich/FZJ_KWS2_Lens/FZJ_KWS2_Lens.instr
parsing  #121: mcstas-comps/examples/FZ_Juelich/FZJ_BenchmarkSfin2/FZJ_BenchmarkSfin2.instr
parsing  #122: mcstas-comps/examples/FZ_Juelich/FZJ_SANS_KWS2_AnySample/FZJ_SANS_KWS2_AnySample.instr
parsing  #123: mcstas-comps/examples/Risoe/linup-3/linup-3.instr
parsing  #124: mcstas-comps/examples/Risoe/linup-7/linup-7.instr
parsing  #125: mcstas-comps/examples/Risoe/linup-5/linup-5.instr
parsing  #126: mcstas-comps/examples/Risoe/linup-6/linup-6.instr
parsing  #127: mcstas-comps/examples/Risoe/linup-2/linup-2.instr
parsing  #128: mcstas-comps/examples/Risoe/linup-4/linup-4.instr
parsing  #129: mcstas-comps/examples/Risoe/linup-1/linup-1.instr
parsing  #130: mcstas-comps/examples/PSI/PSI_DMC_simple/PSI_DMC_simple.instr
parsing  #131: mcstas-comps/examples/PSI/RITA-II/RITA-II.instr
parsing  #132: mcstas-comps/examples/PSI/PSI_Focus/PSI_Focus.instr
parsing  #133: mcstas-comps/examples/PSI/PSI_DMC/PSI_DMC.instr
parsing  #134: mcstas-comps/examples/PSI/PSI_source/PSI_source.instr
parsing  #135: mcstas-comps/examples/DTU/Vin_test/Vin_test.instr
parsing  #136: mcstas-comps/examples/DTU/Vout_test/Vout_test.instr
parsing  #137: mcstas-comps/examples/DTU/Test_FZP_simple/Test_FZP_simple.instr
parsing  #138: mcstas-comps/examples/Union_validation/Incoherent_validation/Incoherent_validation.instr
parsing  #139: mcstas-comps/examples/Union_validation/Single_crystal_validation/Single_crystal_validation.instr
parsing  #140: mcstas-comps/examples/Union_validation/Powder_validation/Powder_validation.instr
parsing  #141: mcstas-comps/examples/SNS/SNS_test/SNS_test.instr
parsing  #142: mcstas-comps/examples/SNS/Mezei_SNS_decoupled_poisoned/Mezei_SNS_decoupled_poisoned.instr
parsing  #143: mcstas-comps/examples/SNS/SNS_ARCS/SNS_ARCS.instr
parsing  #144: mcstas-comps/examples/SNS/SNS_analytic_test/SNS_analytic_test.instr
parsing  #145: mcstas-comps/examples/SNS/Granroth_SNS_decoupled_poisoned/Granroth_SNS_decoupled_poisoned.instr
parsing  #146: mcstas-comps/examples/SNS/SNS_BASIS/SNS_BASIS.instr
parsing  #147: mcstas-comps/examples/SNS/Gallmeier_SNS_decoupled_poisoned/Gallmeier_SNS_decoupled_poisoned.instr

checking  #83: ESS_butterfly_test - OK
checking  #95: SEMSANS_instrument - OK
checking  #145: Granroth_SNS_decoupled_poisoned
    Missing component type (idx 1): SNS_source

checking  #107: ISIS_IMAT - OK
checking  #43: SAFARI_MPISI
    Missing component type (idx 40): Virtual_output

checking  #51: Demonstration
    Missing component type (idx 63): Union_master

checking  #77: BNL_H8_simple
    Missing component type (idx 18): V_sample

checking  #32: ILL_IN5 - OK
checking  #31: ILL_H512_D22 - OK
checking  #21: ILL_H10_IN8 - OK
checking  #10: ILL_H16_IN5 - OK
checking  #16: ILL_H22_VIVALDI - OK
checking  #140: Powder_validation
    Missing component type (idx 8): Union_master

checking  #6: ILL_H142_IN12 - OK
checking  #54: Demo_shape_primitives_simple - OK
checking  #86: ESS_IN5_reprate - OK
checking  #131: RITA_II - OK
checking  #46: Manual_example
    Missing component type (idx 14): Union_master

checking  #61: templateSasView - OK
checking  #82: ESS_butterfly_MCPL_test - OK
checking  #24: ILL_D4 - OK
checking  #20: ILL_H5 - OK
checking  #144: SNS_analytic_test - OK
checking  #64: templateSANS_MCPL - OK
checking  #127: TAS1_C1_Tilt - OK
checking  #50: External_component
    Missing component type (idx 28): Union_master
    Missing component type (idx 30): Union_master

checking  #117: ViewModISIS_test - OK
checking  #97: ILL_H16_Mantid - OK
checking  #18: ILL_H24 - OK
checking  #3: ILL_IN6 - OK
checking  #84: ESS_butterfly_Guide_curved_test - OK
checking  #49: Tagging_demo
    Missing component type (idx 12): Union_master

checking  #121: FZJ_BenchmarkSfin2 - OK
checking  #134: PSI_source - OK
checking  #137: Test_FZP_simple - OK
checking  #101: ILL_IN5_Mantid - OK
checking  #56: Tomography - OK
checking  #38: ILL_H13_IN20 - OK
checking  #29: ILL_H113 - OK
checking  #106: templateSasView_Mantid - OK
checking  #8: ILL_H16 - OK
checking  #109: ISIS_TOSCA_preupgrade - OK
checking  #110: ISIS_HET - OK
checking  #78: BNL_H8
    Missing component type (idx 18): V_sample

checking  #85: ESS_mcpl2hist - OK
checking  #80: ESS_butterfly_tfocus_NOFOCUS_test - OK
checking  #52: Cryostat_example
    Missing component type (idx 28): Union_master

checking  #99: ISIS_SANS2d_Mantid - OK
checking  #143: SNS_ARCS
    Missing component type (idx 0): SNS_source

checking  #138: Incoherent_validation
    Missing component type (idx 7): Union_master

checking  #26: ILL_IN13 - OK
checking  #75: Union_NCrystal_mix_example
    Missing component type (idx 7): Union_master

checking  #0: WOFSANS - OK
checking  #33: ILL_BRISP - OK
checking  #13: ILL_H143_LADI - OK
checking  #45: Time_of_flight
    Missing component type (idx 55): Union_master

checking  #57: template_simple - OK
checking  #114: ISIS_TS2_Brilliance - OK
checking  #25: ILL_SALSA
    Missing component type (idx 208): Union_master

checking  #126: TAS1_Vana - OK
checking  #69: templateTAS - OK
checking  #17: ILL_H15 - OK
checking  #55: templateNMX - OK
checking  #135: Vin_test
    Missing component type (idx 1): Virtual_input

checking  #70: mini - OK
checking  #4: ILL_H22 - OK
checking  #120: FZJ_KWS2_Lens - OK
checking  #53: SE_usage_example
    Missing component type (idx 10): Union_master

checking  #142: Mezei_SNS_decoupled_poisoned
    Missing component type (idx 1): ESS_moderator_short

checking  #73: templateSANS - OK
checking  #119: ISIS_test - OK
checking  #105: templateSANS2_Mantid - OK
checking  #139: Single_crystal_validation
    Missing component type (idx 10): Union_master

checking  #44: SAFARI_PITSI - OK
checking  #102: ILL_H16_IN5_Mantid - OK
checking  #65: templateDIFF - OK
checking  #30: ILL_H142_D33 - OK
checking  #90: RTP_SANS - OK
checking  #81: ESS_Testbeamline_HZB_V20 - OK
checking  #40: HZB_NEAT - OK
checking  #125: TAS1_Diff_Powder - OK
checking  #12: ILL_H142 - OK
checking  #35: ILL_IN5_Spots - OK
checking  #92: RTP_DIF - OK
checking  #7: ILL_H8_IN1 - OK
checking  #41: HZB_FLEX - OK
checking  #96: ISIS_TOSCA_preupgrade_Mantid - OK
checking  #15: ILL_H53_IN14 - OK
checking  #88: ESS_BEER_MCPL - OK
checking  #39: ILL_H22_D1B - OK
checking  #14: ILL_H53_D16 - OK
checking  #66: Reflectometer - OK
checking  #63: BTsimple - OK
checking  #2: ILL_H22_D1A - OK
checking  #123: TAS1_Diff_Slit - OK
checking  #118: ISIS_SANS2d - OK
checking  #79: h8_test_legacy
    Missing component type (idx 18): V_sample

checking  #98: SNS_ARCS_Mantid
    Missing component type (idx 0): SNS_source

checking  #48: Sample_picture_replica
    Missing component type (idx 35): Union_master

checking  #42: LLB_6T2 - OK
checking  #103: templateSANS_Mantid - OK
checking  #11: ILL_IN4 - OK
checking  #36: ILL_D2B_noenv - OK
checking  #23: ILL_H25_IN22 - OK
checking  #111: ISIS_Prisma2 - OK
checking  #47: Laue_camera
    Missing component type (idx 10): Union_master

checking  #71: templateSANS2 - OK
checking  #130: PSI_DMC_simple - OK
checking  #5: ILL_Lagrange - OK
checking  #93: SESANS_Delft - OK
checking  #132: PSI_Focus - OK
checking  #28: ILL_D2B - OK
checking  #100: templateVanadiumMultipleScat_Mantid - OK
checking  #59: template - OK
checking  #104: Tools_ONION
    Missing component type (idx 1): SNS_source

checking  #74: Union_NCrystal_example
    Missing component type (idx 9): Union_master
    Missing component type (idx 12): Union_master

checking  #1: ILL_H5_new - OK
checking  #60: test_SasView_guinier - OK
checking  #22: ILL_H22_D1B_noenv - OK
checking  #67: test_SasView_bcc_paracrystal - OK
checking  #9: ILL_H25 - OK
checking  #37: ILL_H15_D11 - OK
checking  #62: templateTOF - OK
checking  #68: templateNMX_TOF - OK
checking  #72: templateLaue - OK
checking  #76: NCrystal_example - OK
checking  #87: ESS_butterfly_tfocus_test - OK
checking  #89: RTP_NeutronRadiography - OK
checking  #94: SEMSANS_Delft - OK
checking  #108: ISIS_TS1_Brilliance - OK
checking  #112: ISIS_GEM - OK
checking  #113: ISIS_CRISP - OK
checking  #115: ISIS_OSIRIS - OK
checking  #122: SANS_KWS2_AnySample - OK
checking  #124: TAS1_Powder - OK
checking  #128: TAS1_Diff_Vana - OK
checking  #129: TAS1_C1 - OK
checking  #133: PSI_DMC - OK
checking  #136: Vout_test
    Missing component type (idx 2): Virtual_output

checking  #146: SNS_BASIS
    Missing component type (idx 1): ESS_moderator_short

checking  #147: Gallmeier_SNS_decoupled_poisoned - OK

Component parse: 352 total, 334 parsed, 6 errors [dupes: 12])
Instrument parse: 148 total, 141 parsed, 0 errors, type-errs: 29 [dupes: 7]


Total time: 416 ms (freq [tsc/t]: 1.80 GHz)
  ParseComponent: 716 µs/hit || 252 ms 60.64% self || 60.64% total || 352 hits
  ParseInstrument: 762 µs/hit || 112 ms 27.12% self || 27.12% total || 148 hits
  CheckInstrument: 44 µs/hit || 6 ms 1.49% self || 1.49% total || 141 hits
</pre>

