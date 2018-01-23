<distribution version="12.0.1" name="FiberSensor" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{573B07C5-5A19-4EEE-AE42-7EB21BDBED51}">
		<general appName="FiberSensor" outputLocation="d:\DTS\CVI\cvidistkit.FiberSensor" relOutputLocation="cvidistkit.FiberSensor" outputLocationWithVars="d:\DTS\CVI\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="true" version="1.0.4">
			<arp company="%company" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="Chinese (Simplified)" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="FiberSensor" welcomeText=""/></userinterface>
		<dirs appDirID="101">
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="FiberSensor" dirID="100" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="FiberSensor" dirID="101" parentID="2" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="d:\DTS\CVI\FiberSensor.uir" relSourcePath="FiberSensor.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="d:\DTS\CVI\cvibuild.FiberSensor\Release\FiberSensor.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="101" projectID="0">
				<fileID>1</fileID></projectOutput>
			<projectDependencies dirID="101" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="FiberSensor" targetFileID="1" destDirID="100" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine cvirte="true" instrsup="true" lvrt="true" analysis="true" netvarsup="true" dotnetsup="true" activeXsup="true" rtutilsup="true" installToAppDir="false"/><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WINXP</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>d:\national instruments\cvi2012\bin\msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="d:\DTS\CVI\FiberSensor.prj" ProjectRelativePath="FiberSensor.prj"/></Projects>
		<buildData progressBarRate="1.692073100201793">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.079506562499998</ProductsAdded>
				<DPConfigured>1.432636875000001</DPConfigured>
				<DPMergeModulesAdded>2.922004312500000</DPMergeModulesAdded>
				<DPClosed>5.103255187499996</DPClosed>
				<DistributionsCopied>5.693958124999996</DistributionsCopied>
				<End>59.099101562499996</End></progressTimes></buildData>
	</msi>
</distribution>
