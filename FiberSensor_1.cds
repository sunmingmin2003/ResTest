<distribution version="13.0.0" name="FiberSensor" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{88558B7B-8159-48A4-9C4D-FF0543CD0EDE}">
		<general appName="FiberSensor" outputLocation="d:\DTS_FibierSensor\FiberSensor\cvibuild.FiberSensor" relOutputLocation="cvibuild.FiberSensor" outputLocationWithVars="d:\DTS_FibierSensor\FiberSensor\cvibuild.FiberSensor" relOutputLocationWithVars="cvibuild.FiberSensor" upgradeBehavior="2" autoIncrement="true" version="1.0.0">
			<arp company="" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="Chinese (Simplified)" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="FiberSensor" welcomeText=""/></userinterface>
		<dirs appDirID="101">
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="FiberSensor" dirID="100" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="FiberSensor" dirID="101" parentID="2" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="DeviceInfo" dirID="102" parentID="101" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="DeviceInfo" dirID="103" parentID="102" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Desktop]" dirID="0" parentID="-1" isMSIDir="true" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="D:\DTS_FibierSensor\FiberSensor\注册.bat" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="d:\DTS_FibierSensor\FiberSensor\FiberSensor.uir" relSourcePath="FiberSensor.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="D:\DTS_FibierSensor\FiberSensor\MSCOMCT2.OCX" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="D:\DTS_FibierSensor\FiberSensor\Alarm.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="D:\DTS_FibierSensor\FiberSensor\光纤传感.jpg" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="D:\DTS_FibierSensor\FiberSensor\DeviceInfo\20151021.csv" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="D:\DTS_FibierSensor\FiberSensor\Alarm.uir" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="D:\DTS_FibierSensor\FiberSensor\DataSaveConfig.cfg" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="8" sourcePath="D:\DTS_FibierSensor\FiberSensor\cvi.ico" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="9" sourcePath="D:\DTS_FibierSensor\FiberSensor\default.ini" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="10" sourcePath="D:\DTS_FibierSensor\FiberSensor\History.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="11" sourcePath="D:\DTS_FibierSensor\FiberSensor\History.uir" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="12" sourcePath="D:\DTS_FibierSensor\FiberSensor\indicate.png" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="13" sourcePath="D:\DTS_FibierSensor\FiberSensor\MAIN.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="14" sourcePath="D:\DTS_FibierSensor\FiberSensor\path.cfg" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="15" sourcePath="D:\DTS_FibierSensor\FiberSensor\RegionName.rnf" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="16" sourcePath="D:\DTS_FibierSensor\FiberSensor\Test.vof" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="17" sourcePath="d:\DTS_FibierSensor\FiberSensor\cvibuild.FiberSensor\Release\FiberSensor.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="101" projectID="0">
				<fileID>17</fileID></projectOutput>
			<projectDependencies dirID="101" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="FiberSensor" targetFileID="17" destDirID="0" cmdLineArgs="" description="" runStyle="NORMAL"/>
			<shortcut name="FiberSensor" targetFileID="17" destDirID="100" cmdLineArgs="" description="" runStyle="NORMAL"/>
			<shortcut name="注册控件" targetFileID="0" destDirID="101" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules>
			<nireports></nireports></mergemodules>
		<products>
			<product name="NI DataSocket 5.1" UC="{81A7E53E-9524-41CE-90D3-7DD3D90B6C58}" productID="{88098056-C07B-4264-A824-C90D5BF3935E}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="DefaultFull" flavorName="Full" verRestr="false" coreVer="5.1.227"/>
			<product name="NI Distributed System Manager 2013" UC="{3DB93F85-A052-4CC0-A7CA-BF8269170235}" productID="{FEA545A1-9CAA-415E-81D8-49951ED44F22}" path="c:\National Instruments Downloads\CVI\2013\" flavorID="DefaultFull" flavorName="Full" verRestr="false" coreVer="13.0.338"/>
			<product name="NI LabWindows/CVI Side-By-Side Run-Time Engine 2013" UC="{F31041D3-8D65-40ED-AC06-6E32191853A9}" productID="{29EF3691-08F2-4B61-BE7D-8FF83BDEB933}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="13.0.632"/>
			<product name="NI Launcher" UC="{7FB990C5-1A51-4C1D-99C4-C6E50368E105}" productID="{3E60279A-618E-4670-81CC-777B8E60991E}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="3.20.351"/>
			<product name="NI Variable Engine 2.6.0" UC="{EB7A3C81-1C0F-4495-8CE5-0A427E4E6285}" productID="{639C20B2-5F6C-4139-96EA-A206EEA6F995}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="DefaultFull" flavorName="Full" verRestr="false" coreVer="2.7.297"/></products>
		<runtimeEngine installToAppDir="false" activeXsup="true" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="true">
			<hasSoftDeps/></runtimeEngine><sxsRuntimeEngine>
			<selected>true</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WINXP</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="d:\DTS_FibierSensor\FiberSensor\FiberSensor.prj" ProjectRelativePath="FiberSensor.prj"/></Projects>
		<buildData progressBarRate="0.524362486086812">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.297301499999922</ProductsAdded>
				<DPConfigured>1.334537999999902</DPConfigured>
				<DPMergeModulesAdded>7.052221249999902</DPMergeModulesAdded>
				<DPClosed>19.472036999999968</DPClosed>
				<DistributionsCopied>24.606125500000047</DistributionsCopied>
				<End>190.707769250000010</End></progressTimes></buildData>
	</msi>
</distribution>
